#pragma once

#include "Lecture.h"

/**
 * @brief Une carte 
 */
typedef struct {
	int* bleu; ///< Tableau des identifiants des animaux bleus (bas->haut).
    int nb_bleu;
    int* rouge;
    int nb_rouge;
} Card;

void initCard(Card* c);
void card_free(Card* c);
int fact(int n);
void gen_cards_rec(int n, int depth, int* perm, int* used, Card* out, int* out_count);
Card* build_all_cards(int n_animaux, int* out_nb_cards);
void shuffle_cards(Card* cards, int n);
void enregistrement(int n, int* A, Card* out, int* out_count);
void afficher_permutation(int n, Card* A);
void heap_iteratif(int n, int* A, Card* out, int* out_count);
