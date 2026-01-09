#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "jeu.h"


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
void fin_partie(int n_joueurs, const char* joueurs, int* scores, Jeu* depart, Jeu* objectif);
void fin(int nb_cards, Card* deck, Lecture* lec, int* scores, int* can_play);

int main(int argc, const char* argv[]) {

    if (argc < 3) {
        printf("Erreur: au moins 2 joueurs en parametres.\n");
        return 1;
    }

    int n_joueurs = argc - 1;
    const char** joueurs = (const char**)argv + 1;

    if (!joueurs_distincts(n_joueurs, joueurs)) {
        printf("Erreur: joueurs non distincts.\n");
        return 1;
    }

    Lecture lec;
    if (!lecture_load(&lec, ".\\crazy.cfg")) {
        return 1;
    }

    int nb_ordres = lec.allow_KI + lec.allow_LO + lec.allow_SO + lec.allow_NI + lec.allow_MA;
    if (lec.n_animaux < 2 || nb_ordres < 3) {
        printf("Erreur: configuration invalide (animaux>=2, ordres>=3).\n");
        lecture_free(&lec);
        return 1;
    }

    print_ordres(&lec);

    srand((unsigned)time(NULL));

    int nb_cards = 0;
    Card* deck = build_all_cards(lec.n_animaux, &nb_cards);

    if (!deck || nb_cards < 2) {
        printf("Erreur: impossible de generer les positions.\n");
        lecture_free(&lec);
        return 1;
    }
    shuffle_cards(deck, nb_cards);

    /* ✅ scores à 0 */
    int* scores = (int*)calloc((size_t)n_joueurs, sizeof(int));
    int* can_play = (int*)malloc((size_t)n_joueurs * sizeof(int));

    if (!scores || !can_play) {
        printf("Erreur memoire.\n");
        fin(nb_cards, deck, &lec, scores, can_play);
        return 1;
    }

    int deck_i = 0;

    Jeu depart, objectif;
    int objectif_valide = 0;

    if (!jeu_from_card(&depart, &lec, &deck[deck_i++])) {
        printf("Erreur init depart.\n");
        fin(nb_cards, deck, &lec, scores, can_play);
        return 1;
    }

    if (!jeu_from_card(&objectif, &lec, &deck[deck_i++])) {
        printf("Erreur init objectif.\n");
        jeu_free(&depart);
        fin(nb_cards, deck, &lec, scores, can_play);
        return 1;
    }
    objectif_valide = 1;

    affichage(&depart, &objectif, &lec);

    char line[2048];
    int game_over = 0;

    while (!game_over) {

        for (int i = 0; i < n_joueurs; i++)
            can_play[i] = 1;

        /* plus de carte objectif ? */
        if (deck_i >= nb_cards) {
            break;
        }

        while (fgets(line, (int)sizeof(line), stdin) != NULL) {

            if (line[0] == '\n' || (line[0] == '.' && (line[1] == '\n' || line[1] == '\0')))
                continue;

            char id[128], seq[1024];
            int nread = sscanf(line, " %127s %1023s", id, seq);
            if (nread < 2) {
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

            /* ✅ utilise la fonction déjà faite dans jeu.c */
            int ok_obj = sequence_reussit(&depart, &objectif, &lec, seq);

            if (ok_obj) {
                scores[pj] += 1;
                printf("%s gagne un point\n", id);

                /* ✅ MOVE PROPRE : objectif devient depart */
                jeu_free(&depart);
                depart = objectif;
                objectif_valide = 0; /* objectif n'est plus "indépendant" jusqu'à recréation */

                /* nouvelle carte objectif */
                if (deck_i >= nb_cards || !jeu_from_card(&objectif, &lec, &deck[deck_i++])) {
                    game_over = 1;
                    break;
                }
                objectif_valide = 1;

                affichage(&depart, &objectif, &lec);
                break;
            }
            else {
                can_play[pj] = 0;
                printf("Sequence invalide: %s ne peut plus jouer durant ce tour\n", id);

                int last = -1, nb_ok = 0;
                for (int i = 0; i < n_joueurs; i++) {
                    if (can_play[i]) { nb_ok++; last = i; }
                }

                if (nb_ok == 1) {
                    scores[last] += 1;
                    printf("%s gagne un point car lui seul peut encore jouer durant ce tour\n", joueurs[last]);

                    /* ✅ MOVE PROPRE */
                    jeu_free(&depart);
                    depart = objectif;
                    objectif_valide = 0;

                    if (deck_i >= nb_cards || !jeu_from_card(&objectif, &lec, &deck[deck_i++])) {
                        game_over = 1;
                        break;
                    }
                    objectif_valide = 1;

                    affichage(&depart, &objectif, &lec);
                    break;
                }
            }
        }

        if (feof(stdin)) break;
    }

    /* Fin de partie */
    fin_partie(n_joueurs, joueurs, scores, &depart, (objectif_valide ? &objectif : NULL));
    fin(nb_cards, deck, &lec, scores, can_play);
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

void fin_partie(int n_joueurs, const char* joueurs, int* scores, Jeu* depart, Jeu* objectif) {
    print_scores_fin(n_joueurs, joueurs, scores);

    if (depart)  jeu_free(depart);
    if (objectif) jeu_free(objectif);
}

void fin(int nb_cards, Card* deck, Lecture* lec, int* scores, int* can_play) {
    if (deck) {
        for (int i = 0; i < nb_cards; i++) card_free(&deck[i]);
        free(deck);
    }
    if (lec) lecture_free(lec);
    free(scores);
    free(can_play);
}