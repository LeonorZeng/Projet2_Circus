#pragma once

#include "itemliste.h"

/**
 * @brief Maillon d'une liste chaînée. 
 */
struct _Maillon_ {
	ItemL element;              ///< Valeur de l'élément.
	struct _Maillon_* suivant; ///< Adresse du maillon suivant (ou <code>NULL</code>).
};

// Facilité d'écriture.
typedef struct _Maillon_ Maillon;

/**
 * @brief Liste chaînée de valeurs (stockées au sein de maillons).
 */
typedef struct {
	Maillon* premier; ///< Adresse du premier maillon.
} Liste;

/**
 * @brief Initialise une liste comme étant vide. Pour éviter toute fuite mémoire, toute liste
 * non vide doit être détruite lorsqu'elle ne sera plus utilisée (@ref detruireListe).
 * @param[out] li Adresse de la liste devant être initialisée.
 */
void initListe(Liste* li);

/**
 * @brief Indique si une liste est vide.
 * @param[in] li Adresse de la liste.
 * @return 1 si la liste est vide et 0 dans le cas contraire.
 */
int estVide(const Liste* li);

/**
 * @brief Insère un nouvel élément au début d'une liste.
 * @param[in,out] li Adresse de la liste.
 * @param[in] it Elément devant être ajouté.
 * @return 1 en cas de succès et 0 en cas de mémoire insuffisante. Dans ce dernier cas, 
 * la liste est inchangée.
 */
int inclure(Liste* li, ItemL it);

/**
 * @brief Retourne le premier élément d'une liste.
 * @param[in] li Adresse de la liste.
 * @return Le premier élément de la liste.
 * @pre La liste ne doit pas être vide.
 */
ItemL premier(const Liste* li);

/**
 * @brief Supprime le premier élément d'une liste.
 * @param[in,out] li Adresse de la liste.
 * @pre La liste ne doit pas être vide.
 */
void tronquer(Liste* li);

/**
 * @brief Vide une liste (i.e. supprime tous ses éléments).
 * @param[in,out] li Adresse de la liste.
 */
void detruireListe(Liste* li);

/**
 * @brief Itérateur sur une liste permettant d'obtenir un à un les 
 * éléments qui la composent mais pas de modifier cette liste.
 * Désigne le maillon courant (ou vaut NULL si le parcours est fini).
 */
typedef const Maillon* ConstIt;

/**
 * @brief Retourne un itérateur de liste au début du parcours.
 * @param[in] li Adresse de la liste à parcourir.
 * @return Un itérateur initialisé au début de cette liste.
 */
ConstIt iterer(const Liste* li);

/**
 * @brief Indique si tous les éléments ont été parcourus.
 * @param[in] iter Adresse de l'itérateur de liste.
 * @return 1 si tout a été parcouru et 0 dans le cas contraire.
 */
int fini(const ConstIt* iter);

/**
 * @brief Retourne l'élément courant du parcours.
 * @param[in] iter Adresse de l'itérateur de liste.
 * @return L'élément de la liste actuellement désigné par l'itérateur.
 * @pre Le parcours ne doit pas être fini (@ref fini).
 */
ItemL courant(const ConstIt* iter);

/**
 * @brief Passe à l'élément suivant dans le parcours.
 * @param[in,out] iter Adresse de l'itérateur de liste.
 * @pre Le parcours ne doit pas être fini (@ref fini).
 */
void suivant(ConstIt* iter);

/**
 * @brief Itérateur sur une liste permettant d'obtenir un à un les
 * éléments qui la composent mais aussi de modifier cette liste
 * (modification de la valeur de l'élément courant, insertion avant
 * l'élément courant, suppression de l'élément courant).
 */
typedef struct {
	Liste* liste; ///< La liste étant parcourue.
	Maillon* pred; ///< Désigne le maillon précédent le maillon courant (ou NULL).
	Maillon* courant; ///< Désigne le maillon courant (ou NULL).
} Iterateur;

/**
 * @brief Initialise un itérateur de liste au début du parcours.
 * @param[in] li Adresse de la liste dont les éléments doivent être parcourus.
 * @param[out] iter Adresse de l'itérateur à initialiser.
 */
void initIterateur(Liste* li, Iterateur* iter);

/**
 * @brief Indique si tous les éléments ont été parcourus.
 * @param[in] iter Adresse de l'itérateur de liste.
 * @return 1 si tout a été parcouru et 0 dans le cas contraire.
 */
int finiIt(const Iterateur* iter);

/**
 * @brief Retourne l'élément courant du parcours.
 * @param[in] iter Adresse de l'itérateur de liste.
 * @return L'élément de la liste actuellement désigné par l'itérateur.
 * @pre Le parcours ne doit pas être fini (@ref finiIt).
 */
ItemL courantIt(const Iterateur* iter);

/**
 * @brief Passe à l'élément suivant dans le parcours.
 * @param[in,out] iter Adresse de l'itérateur de liste.
 * @pre Le parcours ne doit pas être fini (@ref finiIt).
 */
void suivantIt(Iterateur* iter);

/**
 * @brief Modifie l'élément courant étant parcouru.
 * @param[in] iter Adresse de l'itérateur de liste.
 * @param it Nouvelle valeur de l'élément courant.
 * @pre Le parcours ne doit pas être fini (@ref finiIt).
 */
void changer(const Iterateur* iter, ItemL it);

/**
 * @brief Insère un nouvel élément avant l'élément courant du parcours
 * et, en fin de liste, si le parcours est fini (@ref finiIt). Le nouvel
 * élément courant est celui ayant été inséré.
 * @param[in,out] iter Adresse de l'itérateur de liste.
 * @param[in] it La valeur du nouvel élément.
 * @return 1 en cas de succès et 0 en cas de mémoire insuffisante. Dans ce dernier cas,
 * la liste est inchangée.
 */
int inserer(Iterateur* iter, ItemL it);

/**
 * @brief Supprime l'élément courant du parcours et positionne l'itérateur 
 * sur l'élément suivant.
 * @param[in,out] iter Adresse de l'itérateur de liste.
 * @return L'élément ayant été supprimé de la liste.
 * @pre Le parcours ne doit pas être fini (@ref finiIt).
 */
ItemL effacer(Iterateur* iter);
