#pragma once

/**
 * @brief Une carte qui representre une position d'animaux
 * avec les podiums bleu et rouge.  
 */
typedef struct {
	int* bleu; ///< Tableau des identifiants des animaux bleus (bas->haut).
	int nb_bleu; ///< Nombre d'animaux bleus.
	int* rouge; ///< Tableau des identifiants des animaux rouges (bas->haut).
	int nb_rouge; ///< Nombre d'animaux rouges.
} Card;

/** @brief Initialise une carte vide.
 * @param[out] c L'adresse de la carte ¨¤ initialiser.
 */
void initCard(Card* c);

/** @brief Lib¨¨re toute la m¨¦moire dynamique associ¨¦e ¨¤ une carte.
 * @param[in,out] c L'adresse de la carte ¨¤ lib¨¦rer.
 */
void card_free(Card* c);

/** @brief Calcule la factorielle d'un entier n >= 0.
 * @param[in] n Le nombre dont on veut calculer la factorielle.
 * @return le factorielle de n.
 */
int fact(int n);

/** @brief G¨¦n¨¨re toutes les cartes possibles pour un nombre donn¨¦ d'animaux.
 * @param[in] n_animaux Le nombre d'animaux.
 * @param[out] out_nb_cards L'adresse o¨´ stocker le nombre de cartes g¨¦n¨¦r¨¦es.
 * @return Un tableau dynamique contenant toutes les cartes g¨¦n¨¦r¨¦es,
 * ou NULL en cas d'erreur m¨¦moire.
 * @note La m¨¦moire allou¨¦e pour les cartes doit ¨ºtre lib¨¦r¨¦e par l'appelant
 * en utilisant la fonction @ref card_free pour chaque carte, puis en lib¨¦rant
 * le tableau lui-m¨ºme.
 */
Card* build_all_cards(int n_animaux, int* out_nb_cards);

/** @brief M¨¦lange un tableau de cartes en place avec le m¨¦lange Fisher-Yates.
 * @param[in,out] cards Le tableau de cartes ¨¤ m¨¦langer.
 * @param[in] n Le nombre de cartes dans le tableau.
 */
void shuffle_cards(Card* cards, int n);

/** @brief Enregistre une permutation d'animaux en diff¨¦rentes cartes
 * en divisant la permutation en podiums bleu et rouge.
 * @param[in] n Le nombre total d'animaux.
 * @param[in] animaux Le tableau repr¨¦sentant la permutation des animaux.
 * @param[out] out Le tableau o¨´ stocker les cartes g¨¦n¨¦r¨¦es.
 * @param[out] out_count L'adresse o¨´ stocker le nombre de cartes g¨¦n¨¦r¨¦es.
 */
void enregistrement(int n, int* animaux, Card* out, int* out_count);

/** @brief Affiche les cartes g¨¦n¨¦r¨¦es avec le id des animaux.
 * @param[in] n Le nombre de cartes ¨¤ afficher.
 * @param[in] animaux Le tableau des cartes ¨¤ afficher.
 */
void afficher_permutation(int n, Card* animaux);

/** @brief G¨¦n¨¨re toutes les permutations d'un tableau d'animaux
 * en utilisant l'algorithme de Heap de mani¨¨re it¨¦rative,
 * et enregistre chaque permutation en diff¨¦rentes cartes.
 * @param[in] n Le nombre total d'animaux.
 * @param[in] animaux Le tableau repr¨¦sentant la permutation initiale des animaux.
 * @param[out] out Le tableau o¨´ stocker les cartes g¨¦n¨¦r¨¦es.
 * @param[out] out_count L'adresse o¨´ stocker le nombre de cartes g¨¦n¨¦r¨¦es.
 */
void heap_iteratif(int n, int* animaux, Card* out, int* out_count);
