#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "card.h"

void initCard(Card* c) {
    if (!c) return;
    c->bleu = NULL;
    c->nb_bleu = 0;
    c->rouge = NULL;
    c->nb_rouge = 0;
}

void card_free(Card* c) {
    if (!c) return;
    free(c->bleu);
    c->bleu = NULL;
    c->nb_bleu = 0;
    free(c->rouge);
    c->rouge = NULL;
    c->nb_rouge = 0;
}

int fact(int n) {
    for (int i = n - 1; i >= 1 ; --i)
        n *= i;
    return n;
}

Card* build_all_cards(int n_animaux, int* out_nb_cards) {
    int nb = fact(n_animaux + 1);
    Card* cards = (Card*)malloc(sizeof(Card) * nb);
    if (!cards) 
        return NULL;

    int* perm = (int*)malloc(n_animaux * sizeof(int));
    if (!perm) {
        free(cards);
        printf("Erreur memoire. Nous ne pouvons pas g¨¦n¨¦rer de carte\n");
        return NULL;
    }
    
    for (int i = 0; i < n_animaux; ++i)
        perm[i] = i;

    int count = 0;
    heap_iteratif(n_animaux, perm, cards, &count);

    free(perm);

    *out_nb_cards = count;
    printf("Nombre de cartes generees: %d\n\n", count);
    return cards;
}

void shuffle_cards(Card* cards, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card tmp = cards[i];
        cards[i] = cards[j];
        cards[j] = tmp;
    }
}

void afficher_permutation(int n, Card* animaux) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < animaux[i].nb_bleu; j++) {
            printf("%d ", animaux[i].bleu[j]);
        }

        printf("| ");

        for (int r = 0; r < animaux[i].nb_rouge; r++){
            printf("%d ", animaux[i].rouge[r]);
        }
        printf("\n");
    }
}

void enregistrement(int n, const int* perm, Card* out, int* out_count) {
    for (int j = 0; j <= n; ++j) {
        Card* c = &out[*out_count];
        c->nb_bleu = j;
        c->nb_rouge = n - j;

        c->bleu = (j > 0) ? (int*)malloc((size_t)j * sizeof(int)) : NULL;
        c->rouge = (n - j > 0) ? (int*)malloc((size_t)(n - j) * sizeof(int)) : NULL;

        if ((j > 0 && !c->bleu) || (n - j > 0 && !c->rouge)) {
            free(c->bleu);
            free(c->rouge);
            c->bleu = NULL;
            c->rouge = NULL;
            c->nb_bleu = 0;
            c->nb_rouge = 0;
            (*out_count)++;
            continue;
        }

        for (int i = 0; i < j; ++i) 
            c->bleu[i] = perm[i];
        for (int i = 0; i < n - j; ++i) 
            c->rouge[i] = perm[j + i];

        (*out_count)++;
    }
}

void heap_iteratif(int n, int* animaux, Card* out, int* out_count) {
    int* compteur = (int*)calloc(n, sizeof(int));
    if (!compteur) {
        printf("Erreur memoire. Nous ne pouvons pas generer les cartes.\n");
        return;
	}

    enregistrement(n, animaux, out, out_count);

    int i = 0;

    while (i < n) {
        if (compteur[i] < i) {
			int swap = (i % 2 == 0) ? 0 : compteur[i];
            int tmp = animaux[swap];
            animaux[swap] = animaux[i];
            animaux[i] = tmp;

            enregistrement(n, animaux, out, out_count);

            ++compteur[i]; // on incr¨¦mente l'indice de boucle apr¨¨s avoir effectu¨¦ une it¨¦ration
            i = 0;// on retourne au cas de base de la version r¨¦cursive
        }

        else {
            // la boucle de niveau i est termin¨¦e, on peut donc r¨¦initialiser l'indice et retourner au niveau sup¨¦rieur
            compteur[i] = 0;
            ++i;
        }
    }
	free(compteur);

}