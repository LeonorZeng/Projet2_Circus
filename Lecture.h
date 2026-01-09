#pragma once
#include <stdio.h>

/**
 * @brief Structure contenant la configuration lue depuis crazy.cfg
 * sur les animaux et les ordres autorisés.
 */
typedef struct {
    char** animaux;
    int n_animaux;

    int allow_KI;
    int allow_LO;
    int allow_SO;
    int allow_NI;
    int allow_MA;
} Lecture;

/* ------------------------------------------------------------
   Lire UNE ligne complète en concaténant des blocs fgets.
   Renvoie une chaîne allouée (à free) ou NULL si EOF.
   ------------------------------------------------------------ */
char* read_line_concat(FILE* f);
void free_words(char** words, int n);
char* dup_word(const char* start, int len);
int split_words(const char* line, char*** out_words, int* out_n);
void reset_orders(Lecture* lec);
int apply_order_token(Lecture* lec, const char* tok);

/**
 * @brief Charge la configuration depuis un fichier.
 * @param[out] lec L'adresse de la structure Lecture à remplir.
 * @param[in] filename Le nom du fichier à lire.
 * @return 1 si succès, 0 en cas d'erreur (fichier inaccessible ou format invalide).
 */
int lecture_load(Lecture* lec, const char* filename);

/**
 * @brief Libère toute la mémoire dynamique associée à une structure Lecture.
 * @param[in,out] lec L'adresse de la structure Lecture à libérer.
 */
void lecture_free(Lecture* lec);

/**
 * @brief Retourne le nom de l'animal correspondant à un identifiant.
 * @param[in] lec L'adresse de la structure Lecture contenant les animaux.
 * @param[in] id L'identifiant de l'animal.
 * @return Le nom de l'animal, ou une chaîne indiquant que l'id n'est pas valide.
 */
const char* name_from_id(const Lecture* lec, int id);

/**
 * @brief Affiche les ordres autorisés selon la configuration.
 * @param[in] lec L'adresse de la structure Lecture contenant les ordres.
 */
void print_ordres(const Lecture* lec);

