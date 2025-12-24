#pragma once

#include "itemvecteur.h"

/**
 * @brief Conteneur stockant des éléments accessibles en 
 * fonction de leur position (indice).
 */
typedef struct {
	int nbElements; ///< Nombre d'éléments présents dans le vecteur.
	int capacite;	///< Nombre d'éléments maximal du vecteur.
	ItemV* elements; ///< Tableau (dynamique) de taille <code>capacite</code>.
} Vecteur;

/**
 * @brief Initialise un vecteur d'une capacité donnée contenant aucun élément.
 * Après son utilisation, la mémoire occupée par un vecteur doit être libérée 
 * en invoquant la fonction @ref detruireVecteur.
 * @param[out] v L'adresse du vecteur à initialiser.
 * @param[in] capacite La capacité initiale du vecteur.
 * @return 0 en cas d'échec (manque de mémoire disponible) et 1 en cas de succès.
 * @pre <code>capacite</code> doit être supérieur ou égal à 1.
 */
int initVecteur(Vecteur* v, int capacite);

/**
 * @brief Retourne le nombre d'éléments présents dans un vecteur.
 * @param[in] v L'adresse du vecteur.
 * @return Le nombre d'éléments contenu dans <code>v</code>.
 */
int taille(const Vecteur* v);

/**
 * @brief Ajoute un élément dans un vecteur. Cet élément est ajouté après ceux déjà présents.
 * @param[in,out] v L'adresse du vecteur.
 * @param[in] it L'élément devant être ajouté.
 * @return 0 en cas d'échec (manque de mémoire disponible) et 1 en cas de succès.
 */
int ajouter(Vecteur* v, ItemV it);

/**
 * @brief Retourne l'élément d'un vecteur se trouvant à une position donnée.
 * @param[in] v L'adresse du vecteur. 
 * @param[in] i La position (i.e. l'indice).
 * @return L'élément de <code>v</code> se trouvant à l'indice <code>i</code>.
 * @pre La valeur de <code>i</code> doit être comprise entre 0 et 
 * <code>(taille(v) - 1)</code> (inclus).
 */
ItemV obtenir(const Vecteur* v, int i);

/**
 * @brief Modifie un élément d'un vecteur.
 * @param[in,out] v L'adresse du vecteur.
 * @param[in] i La position (i.e. l'indice) de l'élément devant être modifié.
 * @param[in] it La nouvelle valeur de l'élément.
 * @pre La valeur de <code>i</code> doit être comprise entre 0 et
 * <code>(taille(v) - 1)</code> (inclus).
 */
void modifier(Vecteur* v, int i, ItemV it);

/**
 * @brief Supprime un élément d'un vecteur.
 * @param[in,out] v L'adresse du vecteur.
 * @param[in] i La position (i.e. l'indice) de l'élément devant être supprimé.
 * @pre La valeur de <code>i</code> doit être comprise entre 0 et
 * <code>(taille(v) - 1)</code> (inclus).
 */
void supprimer(Vecteur* v, int i);

/**
 * @brief Modifie la capacité courante d'un vecteur. Si la nouvelle capacité du vecteur 
 * excède le nombre d'éléments qu'il contient, tous ses éléments sont conservés. Dans
 * le cas contraire, les éléments excédentaires sont perdus.
 * @param[in,out] v L'adresse du vecteur.
 * @param[in] taille La nouvelle capacité.
 * @return 0 en cas d'échec (manque de mémoire disponible) et 1 en cas de succès.
 * @pre La nouvelle capacité doit être supérieure ou égale à 1.
 */
int retailler(Vecteur* v, int taille);

/**
 * @brief Libère l'espace mémoire occupé par un vecteur. Après avoir été détruit, il ne doit
 * pas être ré-employé sans avoir été ré-initialisé. Toute autre opération peut donner des 
 * résultats incohérent ou même provoquer l'arrêt brutal du programme.
 * @param[in,out] v L'adresse du vecteur.
 */
void detruireVecteur(Vecteur* v);
