#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lecture.h"
#include "jeu.h"

/* =========================
   Outils joueurs / scores
   ========================= */

static int joueurs_distincts(int n, const char* noms[]) {
    for (int i = 0; i < n; i++) {
        for (int k = i + 1; k < n; k++) {
            if (strcmp(noms[i], noms[k]) == 0) return 0;
        }
    }
    return 1;
}

static int index_joueur(int n, const char* noms[], const char* id) {
    for (int i = 0; i < n; i++) {
        if (strcmp(noms[i], id) == 0) return i;
    }
    return -1;
}

/* Tri des scores: score décroissant, puis nom croissant */
typedef struct {
    const char* nom; /* pointe vers argv */
    int score;
} ScoreLine;

static int cmp_scoreline(const void* a, const void* b) {
    const ScoreLine* A = (const ScoreLine*)a;
    const ScoreLine* B = (const ScoreLine*)b;

    if (A->score != B->score) return (B->score - A->score); /* décroissant */
    return strcmp(A->nom, B->nom);                           /* alphabétique */
}

static void print_scores_fin(int n, const char* noms[], const int scores[]) {
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

/* =========================
   Ligne d'ordres autorisés
   ========================= */
   /* Doit afficher la ligne en se limitant aux ordres autorisés :contentReference[oaicite:9]{index=9} */
static void print_ligne_ordres(const Lecture* lec) {
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

/* =========================
   Génération des "cartes position"
   (toutes les positions possibles)
   ========================= */

   /* Une carte = contenu bleu + contenu rouge (bas->haut), avec des ids d'animaux [0..n-1] */
typedef struct {
    int* bleu;
    int nb_bleu;
    int* rouge;
    int nb_rouge;
} Card;

static void card_free(Card* c) {
    free(c->bleu);
    free(c->rouge);
    c->bleu = NULL; c->rouge = NULL;
    c->nb_bleu = 0; c->nb_rouge = 0;
}

/* Factorielle (petit n) */
static int fact(int n) {
    int r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

/* Construit toutes les cartes:
   pour chaque permutation perm[0..n-1], pour chaque split k:
     bleu = perm[0..k-1], rouge = perm[k..n-1]
   => (n+1)*n! cartes (24 si n=3) :contentReference[oaicite:10]{index=10}
*/
static void gen_cards_rec(int n, int depth, int* perm, int* used,
    Card* out, int* out_count) {
    if (depth == n) {
        for (int k = 0; k <= n; k++) {
            Card* c = &out[*out_count];

            c->nb_bleu = k;
            c->nb_rouge = n - k;

            c->bleu = (k > 0) ? (int*)malloc((size_t)k * sizeof(int)) : NULL;
            c->rouge = (n - k > 0) ? (int*)malloc((size_t)(n - k) * sizeof(int)) : NULL;

            if ((k > 0 && !c->bleu) || (n - k > 0 && !c->rouge)) {
                /* si manque mémoire: on met une carte vide (et on continue) */
                free(c->bleu); free(c->rouge);
                c->bleu = NULL; c->rouge = NULL;
                c->nb_bleu = 0; c->nb_rouge = 0;
            }
            else {
                for (int i = 0; i < k; i++) c->bleu[i] = perm[i];
                for (int i = 0; i < n - k; i++) c->rouge[i] = perm[k + i];
            }

            (*out_count)++;
        }
        return;
    }

    for (int x = 0; x < n; x++) {
        if (!used[x]) {
            used[x] = 1;
            perm[depth] = x;
            gen_cards_rec(n, depth + 1, perm, used, out, out_count);
            used[x] = 0;
        }
    }
}

static Card* build_all_cards(int n_animaux, int* out_nb_cards) {
    int nb = (n_animaux + 1) * fact(n_animaux);
    Card* cards = (Card*)calloc((size_t)nb, sizeof(Card));
    if (!cards) return NULL;

    int* perm = (int*)malloc((size_t)n_animaux * sizeof(int));
    int* used = (int*)calloc((size_t)n_animaux, sizeof(int));
    if (!perm || !used) {
        free(perm); free(used);
        free(cards);
        return NULL;
    }

    int count = 0;
    gen_cards_rec(n_animaux, 0, perm, used, cards, &count);

    free(perm);
    free(used);

    *out_nb_cards = count; /* normalement nb */
    return cards;
}

/* Mélange Fisher-Yates */
static void shuffle_cards(Card* cards, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card tmp = cards[i];
        cards[i] = cards[j];
        cards[j] = tmp;
    }
}

/* Convertit une Card vers un Jeu (podiums remplis bas->haut) */
static int jeu_from_card(Jeu* j, const Lecture* lec, const Card* c) {
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

   /* Largeur colonne = max(len noms, 5 (ROUGE), 4 (----)) */
static int max_int(int a, int b) { return (a > b) ? a : b; }

static int max_name_len(const Lecture* lec) {
    int m = 0;
    for (int i = 0; i < lec->n_animaux; i++) {
        int l = (int)strlen(lec->animaux[i]);
        if (l > m) m = l;
    }
    return m;
}

static void print_spaces(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

static void print_center(const char* s, int width) {
    int len = (int)strlen(s);
    int left = (width - len) / 2;
    int right = width - len - left;
    print_spaces(left);
    fputs(s, stdout);
    print_spaces(right);
}

static const char* name_from_id(const Lecture* lec, int id) {
    if (id < 0 || id >= lec->n_animaux) return "";
    return lec->animaux[id];
}

/* Affiche 2 jeux côte à côte selon le format de l’énoncé :contentReference[oaicite:11]{index=11} */
static void print_situation(const Jeu* depart, const Jeu* objectif, const Lecture* lec) {
    int w = max_name_len(lec);
    w = max_int(w, 5);
    w = max_int(w, 4);

    int hb1 = depart->bleu.nbElements;
    int hr1 = depart->rouge.nbElements;
    int h1 = max_int(hb1, hr1);

    int hb2 = objectif->bleu.nbElements;
    int hr2 = objectif->rouge.nbElements;
    int h2 = max_int(hb2, hr2);

    int h = max_int(h1, h2);

    /* lignes animaux: pas de "==>" ici, juste les 4 colonnes */
    for (int row = h - 1; row >= 0; row--) {
        /* depart BLEU */
        if (row < hb1) print_center(name_from_id(lec, depart->bleu.elements[row]), w);
        else print_spaces(w);

        print_spaces(1);

        /* depart ROUGE */
        if (row < hr1) print_center(name_from_id(lec, depart->rouge.elements[row]), w);
        else print_spaces(w);

        print_spaces(1);

        /* objectif BLEU */
        if (row < hb2) print_center(name_from_id(lec, objectif->bleu.elements[row]), w);
        else print_spaces(w);

        print_spaces(1);

        /* objectif ROUGE */
        if (row < hr2) print_center(name_from_id(lec, objectif->rouge.elements[row]), w);
        else print_spaces(w);

        putchar('\n');
    }

    /* ligne dashes avec ==> au milieu */
    print_center("----", w);
    print_spaces(1);
    print_center("----", w);

    printf(" ==> ");

    print_center("----", w);
    print_spaces(1);
    print_center("----", w);
    putchar('\n');

    /* labels */
    print_center("BLEU", w);
    print_spaces(1);
    print_center("ROUGE", w);

    print_spaces(1);

    print_center("BLEU", w);
    print_spaces(1);
    print_center("ROUGE", w);
    putchar('\n');
}

/* =========================
   MAIN
   ========================= */

int main(int argc, const char* argv[]) {
    /* 1) Joueurs : au moins 2, distincts sinon on quitte :contentReference[oaicite:12]{index=12} */
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

    /* 2) Lire crazy.cfg (2 lignes) :contentReference[oaicite:13]{index=13} */
    Lecture lec;
    if (!lecture_load(&lec, "C:\\Users\\nazda\\source\\repos\\Projet2_Circus\\x64\\Debug\\crazy.cfg")
        ) {
        printf("Erreur: fichier crazy.cfg invalide.\n");
        return 0;
    }

    /* vérif: animaux >= 2, ordres >= 3 (sinon stop immédiat) :contentReference[oaicite:14]{index=14} */
    int nb_ordres = lec.allow_KI + lec.allow_LO + lec.allow_SO + lec.allow_NI + lec.allow_MA;
    if (lec.n_animaux < 2 || nb_ordres < 3) {
        printf("Erreur: configuration invalide (animaux>=2, ordres>=3).\n");
        lecture_free(&lec);
        return 0;
    }

    /* 3) Afficher la ligne des ordres autorisés :contentReference[oaicite:15]{index=15} */
    print_ligne_ordres(&lec);

    /* 4) Générer toutes les cartes positions, mélanger, et en consommer sans remise :contentReference[oaicite:16]{index=16} */
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
    int* scores = (int*)calloc((size_t)n_joueurs, sizeof(int));
    int* can_play = (int*)malloc((size_t)n_joueurs * sizeof(int));
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

    /* afficher situation initiale :contentReference[oaicite:17]{index=17} */
    print_situation(&depart, &objectif, &lec);

    /* 7) Boucle de jeu: tant qu’il reste des cartes objectif */
    char line[2048];
    while (1) {
        /* nouveau tour: tout le monde peut jouer */
        for (int i = 0; i < n_joueurs; i++) can_play[i] = 1;

        /* si plus de carte objectif => fin */
        if (deck_i > nb_cards) break; /* sécurité */
        if (deck_i == nb_cards) {
            /* plus de nouvelle carte objectif disponible => partie terminée :contentReference[oaicite:18]{index=18} */
            break;
        }

        /* tour courant: lire des propositions jusqu’à ce que quelqu’un gagne */
        while (fgets(line, (int)sizeof(line), stdin) != NULL) {
            /* ignorer lignes vides / "." (dans l’annexe on voit un "." tapé) :contentReference[oaicite:19]{index=19} */
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

                print_situation(&depart, &objectif, &lec);
                break; /* fin de tour */
            }
            else {
                /* mauvaise séquence -> joueur éliminé du tour :contentReference[oaicite:20]{index=20} */
                can_play[pj] = 0;
                printf("Sequence invalide: %s ne peut plus jouer durant ce tour\n", id);

                /* si un seul joueur peut encore jouer -> il gagne le point :contentReference[oaicite:21]{index=21} */
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

                    print_situation(&depart, &objectif, &lec);
                    break; /* fin de tour */
                }
            }
        }

        /* EOF -> fin */
        if (feof(stdin)) break;
    }

fin_partie:
    /* 8) Fin: afficher scores triés (strict) :contentReference[oaicite:22]{index=22} */
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
