#pragma once
#include "vecteur.h"

typedef Vecteur Podium;

/**
 * @brief Initialise un podium d'une capacité donnée contenant aucun élément.
 * Après son utilisation, la mémoire occupée par un podium doit être libérée
 * en invoquant la fonction @ref podium_free.
 * @param[out] p L'adresse du podium à initialiser.
 * @param[in] cap La capacité initiale du podium.
 * @return 0 en cas d'échec (manque de mémoire disponible) et 1 en cas de succès.
 * @pre <code>cap</code> doit être supérieur ou égal à 1.
 */	
int initPodium(Podium* p, int cap);

/**
 * @brief Libère toute la mémoire dynamique associée à un podium.
 * @param[in,out] p L'adresse du podium à libérer.
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
 * @brief Dépile (pop) un animal d'un podium. L'animal au sommet du podium est retiré.
 * @param[in,out] p L'adresse du podium.
 * @return L'élément dépilé, ou -1 si le podium est vide.
 */
int podium_pop(Podium* p);

/**
 * @brief Affiche le contenu d'un podium (du bas vers le haut).
 * @param[in] p L'adresse du podium à afficher.
 */
void podium_print(const Podium* p);

/**
 * @brief Effectue une rotation du podium du bas vers le haut.
 * L'élément au bas du podium est déplacé au sommet.
 * @param[in,out] p L'adresse du podium à faire tourner.
 * @return 0 si le podium est vide, 1 sinon.
 */
int podium_bas_vers_haut(Podium* p);

/**
 * @brief Clone le contenu d'un podium source vers un podium destination.
 * @param[out] dst L'adresse du podium destination.
 * @param[in] src L'adresse du podium source.
 * @return 0 en cas d'échec (manque de mémoire disponible) et 1 en cas de succès.
 */
int podium_clone(Podium* dst, const Podium* src);

/**
 * @brief Compare deux podiums pour vérifier s'ils sont égaux.
 * Deux podiums sont considérés égaux s'ils ont le même nombre d'éléments
 * et que chaque élément à la même position est identique.
 * @param[in] a L'adresse du premier podium.
 * @param[in] b L'adresse du second podium.
 * @return 1 si les podiums sont égaux, 0 sinon.
 */
int podium_equals(const Podium* a, const Podium* b);