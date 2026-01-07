#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "card.h"
#include "lecture.h"


/* =========================
   G¨¦n¨¦ration des "cartes position"
   (toutes les positions possibles)
   ========================= */

   /* Une carte = contenu bleu + contenu rouge (bas->haut), avec des ids d'animaux [0..n-1] */

void initCard(Card* c) {
    c->bleu = NULL;
    c->nb_bleu = 0;
    c->rouge = NULL;
    c->nb_rouge = 0;
}

void card_free(Card* c) {
    free(c->bleu);
    free(c->rouge);
}

/**
 * @brief Calcule la factorielle d'un entier n >= 0.
 * @param[in] n Le nombre dont on veut calculer la factorielle.
 * @return le factorielle de n.
 */
int fact(int n) {
    for (int i = n - 1; 0 < i; --i)
        n *= i;
    return n;
}

/**
 * @brief Cr¨¦e toutes les permutations d'animaux et g¨¦n¨¨re les cartes
 correspondantes avec l'algorithme de Heaps.
 * @param[in] n L'adresse du podium.
 * @param[in] animal L'¨¦l¨¦ment devant ¨ºtre ajout¨¦.
 * @return 0 si pile pleine. et 1 en cas de succ¨¨s.
 */
void gen_cards_rec(int n, int depth, int* perm, int* used, Card* out, int* out_count) {
    if (depth == n) {
        for (int k = 0; k <= n; k++) {
            Card* c = &out[*out_count];

            c->nb_bleu = k;
            c->nb_rouge = n - k;

            c->bleu = (k > 0) ? (int*)malloc((size_t)k * sizeof(int)) : NULL;
            c->rouge = (n - k > 0) ? (int*)malloc((size_t)(n - k) * sizeof(int)) : NULL;

            if ((k > 0 && !c->bleu) || (n - k > 0 && !c->rouge)) {
                /* si manque m¨¦moire: on met une carte vide (et on continue) */
                free(c->bleu); free(c->rouge);
            }

            else {
                for (int i = 0; i < k; i++)
                    c->bleu[i] = perm[i];
                for (int i = 0; i < n - k; i++)
                    c->rouge[i] = perm[k + i];
            }

            (*out_count)++;
        }
        return;
    }

    for (int x = 0; x < n; x++) {
        if (!used[x]) {
            used[x] = 1;
            perm[depth] = x;
            used[x] = 0;
        }
    }
}

Card* build_all_cards(int n_animaux, int* out_nb_cards) {
    int nb = fact(n_animaux + 1);
    Card* cards = (Card*)malloc(sizeof(Card) * nb);
    if (!cards) return NULL;

    int* perm = (int*)malloc(n_animaux * sizeof(int));
    if (!perm) {
        free(perm);
        free(cards);
        printf("Erreur memoire. Nous ne pouvons pas g¨¦n¨¦rer de carte\n");
        return NULL;
    }

    int count = 0;
    heap_iteratif(n_animaux, perm, cards, &count);

    free(perm);

    *out_nb_cards = count; /* normalement nb */
    printf("Nombre de cartes generees: %d\n", count);
    return cards;
}

/* M¨¦lange Fisher-Yates */
void shuffle_cards(Card* cards, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card tmp = cards[i];
        cards[i] = cards[j];
        cards[j] = tmp;
    }
}

void afficher_permutation(int n, Card* A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < A[i].nb_bleu; j++) {
            printf("%d ", A[i].bleu[j]);
            printf("| ");
        }
        for (int r = 0; r < A[i].nb_rouge; r++){
            printf("%d ", A[i].rouge[r]);
        }
        printf("\n");
    }
}

void enregistrement(int n, int* A, Card* out, int* out_count) {
    Card* c = (Card*)malloc(sizeof(Card));
    initCard(c);
    for (int j = 0; j < n + 1; j++) {
        for (int i = 0; i < n; i++) {
            if (i <= j) {
                c->bleu[i] = A[i];
                ++c->nb_bleu;
            }

            else {
                c->rouge[i] = A[i];
                ++c->nb_rouge;
            }
        }
        out[j] = *c;
        (*out_count)++;
    }
}

void heap_iteratif(int n, int* A, Card* out, int* out_count) {
    int compteur[] = { 0,0,0,0 };

    for (int i = 0; i < n; i++)
        compteur[i] = A[i];
    enregistrement(n, A, out, out_count);

    // i indique le niveau de la boucle en cours d'incr¨¦mentation
    int i = 0;

    while (i < n) {
        int tmp;
        if (compteur[i] < i) {

            if (i % 2 == 0) {
                tmp = A[0];
                A[0] = A[i];
                A[i] = tmp;
                enregistrement(n, A, out, out_count);
            }

            else {
                tmp = A[compteur[i]];
                A[compteur[i]] = A[i];
                A[i] = tmp;
                enregistrement(n, A, out, out_count);
            }

            for (int i = 0; i < n; i++)
                compteur[i] = A[i];

            ++compteur[i]; // on incr¨¦mente l'indice de boucle apr¨¨s avoir effectu¨¦ une it¨¦ration
            i = 0;// on retourne au cas de base de la version r¨¦cursive
        }
        else {
            // la boucle de niveau i est termin¨¦e, on peut donc r¨¦initialiser l'indice et retourner au niveau sup¨¦rieur
            compteur[i] = 0;
            ++i;
        }
    }
    afficher_permutation(*out_count, out);

}