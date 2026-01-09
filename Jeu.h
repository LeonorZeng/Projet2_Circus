#pragma once

#include "podium.h"
#include "lecture.h"
#include "card.h"

/**
 * @brief Conteneur stockant des éléments accessibles en
 * fonction de leur position (indice).
 */
typedef struct {
    Podium bleu;
    Podium rouge;
} Jeu;

/** @brief Initialise un jeu vide.
 * @param[out] j L'adresse du jeu à initialiser.
 */
int  jeu_init(Jeu* j, const Lecture* lec);

/** @brief Libère toute la mémoire dynamique associée à un jeu.
 * @param[in,out] j L'adresse du jeu à libérer.
 */
void jeu_free(Jeu* j);

/** @brief Convertit une Card vers un Jeu (podiums remplis bas->haut)
 * @param[out] j L'adresse du jeu à initialiser.
 * @param[in] lec La configuration de lecture (pour la taille des podiums).
 * @param[in] c La carte source.
 * @return 1 si succès, 0 sinon (erreur mémoire).
 */
int jeu_from_card(Jeu* j, const Lecture* lec, const Card* c);

/** @brief L'ordre qui deplace un animal du haut du podium bleu vers le podium rouge.
 * @param[in,out] j L'adresse du jeu à libérer.
 * @param[in] lec La configuration de lecture (pour vérifier si l'ordre est autorisé).
 */
int ordre_KI(Jeu* j, const Lecture* lec);

/** @brief L'ordre qui deplace un animal du haut du podium rouge vers le podium bleu.
 * @param[in,out] j L'adresse du jeu à libérer.
 * @param[in] lec La configuration de lecture (pour vérifier si l'ordre est autorisé).
 */
int ordre_LO(Jeu* j, const Lecture* lec);

/** @brief L'ordre qui fait une rotation bas->haut du podium bleu.
 * @param[in,out] j L'adresse du jeu à libérer.
 * @param[in] lec La configuration de lecture (pour vérifier si l'ordre est autorisé).
 */
int ordre_SO(Jeu* j, const Lecture* lec);

/** @brief L'ordre qui fait une rotation bas->haut du podium rouge.
 * @param[in,out] j L'adresse du jeu à libérer.
 * @param[in] lec La configuration de lecture (pour vérifier si l'ordre est autorisé).
 */
int ordre_NI(Jeu* j, const Lecture* lec);

/** @brief L'ordre qui fait une rotation bas->haut des deux podiums.
 * @param[in,out] j L'adresse du jeu à libérer.
 * @param[in] lec La configuration de lecture (pour vérifier si l'ordre est autorisé).
 */
int ordre_MA(Jeu* j, const Lecture* lec);

// Applique une séquence collée : "KILOSONIMA"

/** @brief Applique une séquence d'ordres sur un jeu.
 * @param[in,out] j L'adresse du jeu à modifier.
 * @param[in] lec La configuration de lecture (pour vérifier si les ordres sont autorisés).
 * @param[in] seq La séquence d'ordres collée (ex: "KILOSONIMA").
 * @return 1 si la séquence a été appliquée avec succès, 0 sinon (ordre invalide ou non autorisé).
 */
int jeu_appliquer_sequence(Jeu* j, const Lecture* lec, const char* seq);

// Affichage

/** @brief Affiche 2 jeux côte à côte selon le format de l’énoncé.
 * @param[in] depart L'adresse du jeu de départ.
 * @param[in] objectif L'adresse du jeu objectif.
 * @param[in] lec La configuration de lecture (pour les noms des animaux).
 */
void affichage(const Jeu* depart, const Jeu* objectif, const Lecture* lec);

// Copie / comparaison / validation

/** @brief Clone un jeu.
 * @param[out] dst L'adresse du jeu de destination.
 * @param[in] src L'adresse du jeu source.
 * @return 1 si succès, 0 sinon (erreur mémoire).
 */
int jeu_clone(Jeu* dst, const Jeu* src);

/** @brief Compare 2 jeux.
 * @param[in] a L'adresse du premier jeu.
 * @param[in] b L'adresse du second jeu.
 * @return 1 si les jeux sont égaux, 0 sinon.
 */
int jeu_equals(const Jeu* a, const Jeu* b);

/** @brief Vérifie si une séquence d'ordres permet de transformer un jeu de départ en un jeu objectif.
 * @param[in] depart L'adresse du jeu de départ.
 * @param[in] objectif L'adresse du jeu objectif.
 * @param[in] lec La configuration de lecture (pour vérifier les ordres).
 * @param[in] seq La séquence d'ordres collée (ex: "KILOSONIMA").
 * @return 1 si la séquence permet d'atteindre l'objectif, 0 sinon.
 */
int sequence_reussit(const Jeu* depart, const Jeu* objectif,
    const Lecture* lec, const char* seq);

void test_jeu(const Jeu* depart, const Jeu* objectif,
    const Lecture* lec, const char* seq);