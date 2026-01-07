#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lecture.h"
#include "jeu.h"
#include "card.h"


/* Tri des scores: score décroissant, puis nom croissant */
typedef struct {
    const char* nom; //pointe vers argv
    int score;
} ScoreLine;

// Une carte = contenu bleu + contenu rouge (bas->haut), avec des ids d'animaux [0..n-1]

int joueurs_distincts(int n, const char* noms[]);
int index_joueur(int n, const char* noms[], const char* id);
int cmp_scoreline(const void* a, const void* b);
void print_scores_fin(int n, const char* noms[], const int scores[]);
void print_ordres(const Lecture* lec);
int jeu_from_card(Jeu* j, const Lecture* lec, const Card* c);
void affichage(const Jeu* depart, const Jeu* objectif, const Lecture* lec);

int main(int argc, const char* argv[]) {
    /* 1) Joueurs : au moins 2, distincts sinon on quitte : */
    if (argc < 3) {
        printf("Erreur: au moins 2 joueurs en parametres.\n");
        return 0;
    }
    int n_joueurs = argc - 1;
    const char** joueurs = argv + 1;

    if (!joueurs_distincts(n_joueurs, joueurs)) {
        printf("Erreur: joueurs non distincts.\n");
        return 0;
    }

    /* 2) Lire crazy.cfg (2 lignes) : */
    Lecture lec;
    if (!lecture_load(&lec, ".\\crazy.cfg")) {
        return 0;
    }

    /* vérif: animaux >= 2, ordres >= 3 (sinon stop immédiat) :*/
    int nb_ordres = lec.allow_KI + lec.allow_LO + lec.allow_SO + lec.allow_NI + lec.allow_MA;
    if (lec.n_animaux < 2 || nb_ordres < 3) {
        printf("Erreur: configuration invalide (animaux>=2, ordres>=3).\n");
        lecture_free(&lec);
        return 0;
    }

    /* 3) Afficher la ligne des ordres autorisés : */
    print_ordres(&lec);

    /* 4) Générer toutes les cartes positions, mélanger, et en consommer sans remise : */
    srand((unsigned)time(NULL));

    int nb_cards = 0;
    Card* deck = build_all_cards(lec.n_animaux, &nb_cards);

    if (!deck || nb_cards < 2) {
        printf("Erreur: impossible de generer les positions.\n");
        lecture_free(&lec);
        return 0;
    }
    shuffle_cards(deck, nb_cards);

    /* 5) Score + état du tour */
    int* scores = (int*)malloc(n_joueurs * sizeof(int));
    int* can_play = (int*)malloc(n_joueurs * sizeof(int));

    if (!scores || !can_play) {
        printf("Erreur memoire.\n");
        free(scores); free(can_play);
        for (int i = 0; i < nb_cards; i++) card_free(&deck[i]);
        free(deck);
        lecture_free(&lec);
        return 0;
    }

    /* 6) Tirage initial: depart = deck[0], objectif = deck[1] */
    int deck_i = 0;

    Jeu depart, objectif;
    if (!jeu_from_card(&depart, &lec, &deck[deck_i++])) {
        printf("Erreur init depart.\n");
        goto fin;
    }
    if (!jeu_from_card(&objectif, &lec, &deck[deck_i++])) {
        printf("Erreur init objectif.\n");
        jeu_free(&depart);
        goto fin;
    }

    /* afficher situation initiale : */
    affichage(&depart, &objectif, &lec);

    /* 7) Boucle de jeu: tant qu’il reste des cartes objectif */
    char line[2048];
    while (1) {
        /* nouveau tour: tout le monde peut jouer */
        for (int i = 0; i < n_joueurs; i++) can_play[i] = 1;

        /* si plus de carte objectif => fin */
        if (deck_i > nb_cards) break; /* sécurité */
        if (deck_i == nb_cards) {
            /* plus de nouvelle carte objectif disponible => partie terminée :*/
            break;
        }

        /* tour courant: lire des propositions jusqu’à ce que quelqu’un gagne */
        while (fgets(line, (int)sizeof(line), stdin) != NULL) {
            /* ignorer lignes vides / "." (dans l’annexe on voit un "." tapé) :*/
            if (line[0] == '\n' || (line[0] == '.' && (line[1] == '\n' || line[1] == '\0'))) {
                continue;
            }

            char id[128], seq[1024];
            int nread = sscanf(line, " %127s %1023s", id, seq);
            if (nread < 2) {
                /* entrée pas au format "ID SEQ" -> message libre 1 ligne */
                printf("Entree invalide.\n");
                continue;
            }

            int pj = index_joueur(n_joueurs, joueurs, id);
            if (pj < 0) {
                printf("Joueur inconnu.\n");
                continue;
            }

            if (!can_play[pj]) {
                printf("%s ne peut pas jouer\n", id);
                continue;
            }

            /* tester la séquence: on clone depart, on applique, puis on compare à objectif */
            Jeu tmp;
            if (!jeu_clone(&tmp, &depart)) {
                printf("Erreur memoire.\n");
                continue;
            }

            int ok_seq = jeu_appliquer_sequence(&tmp, &lec, seq);
            int ok_obj = ok_seq && jeu_equals(&tmp, &objectif);

            jeu_free(&tmp);

            if (ok_obj) {
                scores[pj] += 1;
                printf("%s gagne un point\n", id);

                /* nouveau départ = objectif atteint */
                jeu_free(&depart);
                depart = objectif; /* copie de struct (les vecteurs restent valides) */

                /* nouvelle carte objectif */
                jeu_free(&objectif);
                if (!jeu_from_card(&objectif, &lec, &deck[deck_i++])) {
                    /* plus de carte => fin */
                    goto fin_partie;
                }

                affichage(&depart, &objectif, &lec);
                break; /* fin de tour */
            }
            else {
                /* mauvaise séquence -> joueur éliminé du tour : */
                can_play[pj] = 0;
                printf("Sequence invalide: %s ne peut plus jouer durant ce tour\n", id);

                /* si un seul joueur peut encore jouer -> il gagne le point : */
                int last = -1;
                int nb_ok = 0;
                for (int i = 0; i < n_joueurs; i++) {
                    if (can_play[i]) { nb_ok++; last = i; }
                }
                if (nb_ok == 1) {
                    scores[last] += 1;
                    printf("%s gagne un point car lui seul peut encore jouer durant ce tour\n", joueurs[last]);

                    /* nouveau départ = objectif (même si personne n’a trouvé la séquence) */
                    jeu_free(&depart);
                    depart = objectif;

                    /* nouvelle carte objectif */
                    jeu_free(&objectif);
                    if (!jeu_from_card(&objectif, &lec, &deck[deck_i++])) {
                        goto fin_partie;
                    }

                    affichage(&depart, &objectif, &lec);
                    break; /* fin de tour */
                }
            }
        }

        /* EOF -> fin */
        if (feof(stdin)) break;
    }

fin_partie:
    /* 8) Fin: afficher scores triés (strict) : */
    print_scores_fin(n_joueurs, joueurs, scores);

    /* lib jeux */
    jeu_free(&depart);
    jeu_free(&objectif);

fin:
    /* lib deck */
    for (int i = 0; i < nb_cards; i++) card_free(&deck[i]);
    free(deck);

    lecture_free(&lec);
    free(scores);
    free(can_play);
    return 0;
}

int joueurs_distincts(int n, const char* noms[]) {
    for (int i = 0; i < n; i++) {
        for (int k = i + 1; k < n; k++) {
            if (strcmp(noms[i], noms[k]) == 0) return 0;
        }
    }
    return 1;
}

int index_joueur(int n, const char* noms[], const char* id) {
    for (int i = 0; i < n; i++) {
        if (strcmp(noms[i], id) == 0) return i;
    }
    return -1;
}

int cmp_scoreline(const void* a, const void* b) {
    const ScoreLine* A = (const ScoreLine*)a;
    const ScoreLine* B = (const ScoreLine*)b;

    if (A->score != B->score) return (B->score - A->score); /* décroissant */
    return strcmp(A->nom, B->nom);                           /* alphabétique */
}

void print_scores_fin(int n, const char* noms[], const int scores[]) {
    ScoreLine* t = (ScoreLine*)malloc((size_t)n * sizeof(ScoreLine));
    if (!t) return;

    for (int i = 0; i < n; i++) {
        t[i].nom = noms[i];
        t[i].score = scores[i];
    }

    qsort(t, (size_t)n, sizeof(ScoreLine), cmp_scoreline);

    for (int i = 0; i < n; i++) {
        printf("%s %d\n", t[i].nom, t[i].score);
    }

    free(t);
}

   /* Doit afficher la ligne en se limitant aux ordres autorisés :*/
void print_ordres(const Lecture* lec) {
    int first = 1;

    /* ordre d'affichage demandé dans le sujet */
    if (lec->allow_KI) {
        if (!first) printf(" | ");
        printf("KI (B -> R)");
        first = 0;
    }
    if (lec->allow_LO) {
        if (!first) printf(" | ");
        printf("LO (B <- R)");
        first = 0;
    }
    if (lec->allow_SO) {
        if (!first) printf(" | ");
        printf("SO (B <-> R)");
        first = 0;
    }
    if (lec->allow_NI) {
        if (!first) printf(" | ");
        printf("NI (B ^)");
        first = 0;
    }
    if (lec->allow_MA) {
        if (!first) printf(" | ");
        printf("MA (R ^)");
        first = 0;
    }

    printf("\n");
}

/* Convertit une Card vers un Jeu (podiums remplis bas->haut) */
int jeu_from_card(Jeu* j, const Lecture* lec, const Card* c) {
    /* initPodium(cap = n_animaux) */
    if (!initPodium(&j->bleu, lec->n_animaux)) return 0;
    if (!initPodium(&j->rouge, lec->n_animaux)) {
        podium_free(&j->bleu);
        return 0;
    }

    for (int i = 0; i < c->nb_bleu; i++) {
        if (!podium_push(&j->bleu, c->bleu[i])) return 0;
    }
    for (int i = 0; i < c->nb_rouge; i++) {
        if (!podium_push(&j->rouge, c->rouge[i])) return 0;
    }
    return 1;
}

/* =========================
   Affichage "situation" (strict)
   - gauche = départ
   - droite = objectif
   - la ligne des dashes contient " ==> "
   ========================= */

int max_name_len(const Lecture* lec) {
    int m = 0;
    for (int i = 0; i < lec->n_animaux; i++) {
        m = (m > (int)strlen(lec->animaux[i])) ? m : (int)strlen(lec->animaux[i]);
    }
    return m;
}

void print_spaces(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

void print_center(const char* s, int width) {
    int len = (int)strlen(s);
    int right = width - len;
    fputs(s, stdout);
    print_spaces(right);
}

/* Affiche 2 jeux côte à côte selon le format de l’énoncé : */
void affichage(const Jeu* depart, const Jeu* objectif, const Lecture* lec) {
    int wr = max_name_len(lec),wb = max_name_len(lec);
    wr = (wr > 5) ? wr : 5;
    wb = (wb > 4) ? wb : 4;

    int hb1 = depart->bleu.nbElements;
    int hr1 = depart->rouge.nbElements;
    int h1 = (hb1 > hr1) ? hb1 : hr1;

    int hb2 = objectif->bleu.nbElements;
    int hr2 = objectif->rouge.nbElements;
    int h2 = (hb2 > hr2) ? hb2 :hr2;

    int h = (h1 > h2) ? h1 : h2;

    /* lignes animaux: pas de "==>" ici, juste les 4 colonnes */
    for (int row = h - 1; row >= 0; row--) {
        /* depart BLEU */
        if (row < hb1) 
            print_center(name_from_id(lec, depart->bleu.elements[row]), wb);
        else 
            print_spaces(wb);

        print_spaces(2);

        /* depart ROUGE */
        if (row < hr1) 
            print_center(name_from_id(lec, depart->rouge.elements[row]), wr);
        else 
            print_spaces(wr);

        print_spaces(2);

        /* objectif BLEU */
        if (row < hb2) 
            print_center(name_from_id(lec, objectif->bleu.elements[row]), wb);
        else 
            print_spaces(wb);

        print_spaces(2);

        /* objectif ROUGE */
        if (row < hr2) 
            print_center(name_from_id(lec, objectif->rouge.elements[row]), wr);
        else 
            print_spaces(wr);

        putchar('\n');
    }

    /* ligne dashes avec ==> au milieu */
    print_center("----", wb);
    print_spaces(2);
    print_center("----", wb);

    printf("  ==>  ");

    print_center("----", wb);
    print_spaces(2);
    print_center("----", wb);
    putchar('\n');

    /* labels */
    print_center("BLEU", wb);
    print_spaces(2);
    print_center("ROUGE", wb);

    print_spaces(2);

    print_center("BLEU", wb);
    print_spaces(2);
    print_center("ROUGE", wb);
    putchar('\n');
}
