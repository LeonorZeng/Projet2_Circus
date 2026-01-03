#pragma once
#include "vecteur.h"

typedef Vecteur Podium;

/**
 * @brief Initialise un podium d'une capacité donnée contenant aucun élément.
 * Après son utilisation, la mémoire occupée par un podium doit être libérée
 * en invoquant la fonction @ref podium_free.
 * @param[out] p L'adresse du podium à initialiser.
 * @param[in] capacite La capacité initiale du podium.
 * @return 0 en cas d'échec (manque de mémoire disponible) et 1 en cas de succès.
 * @pre <code>capacite</code> doit être supérieur ou égal à 1.
 */
int initPodium(Podium* p, int cap);

/**
 * @brief Libère l'espace mémoire occupé par un podium. Après avoir été détruit, il ne doit
 * pas être ré-employé sans avoir été ré-initialisé. Toute autre opération peut donner des
 * résultats incohérent ou même provoquer l'arrêt brutal du programme.
 * @param[in,out] p L'adresse du podium.
 */
void podium_free(Podium* p);

/**
 * @brief Empile (push) un animal sur un podium. Cet animal est ajouté après ceux déjà présents.
 * @param[in,out] p L'adresse du podium.
 * @param[in] animal L'élément devant être ajouté.
 * @return 0 si pile pleine. et 1 en cas de succès.
 */
int podium_push(Podium* p, int animal);

/**
 * @brief Dépile (pop) le sommet d'un podium.
 * @param[in,out] p L'adresse du podium.
 * @return la valeur du dernier élément ajouter
 */
int podium_pop(Podium* p);

// Affichage debug (bas -> haut) en nombres
void podium_print(const Podium* p);

// Rotation bas -> haut (utile pour NI/MA). Renvoie 1 si OK, 0 si <= 1 élément.
int podium_bas_vers_haut(Podium* p);