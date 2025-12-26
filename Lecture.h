#pragma once
#include <stdio.h> 

typedef struct {
    char** animaux;     // tableau dynamique de chaines (noms des animaux)
    int n_animaux;      // nombre d'animaux (>= 3)

    int allow_KI;       // 1 si KI autorisé, 0 sinon
    int allow_LO;       // 1 si LO autorisé, 0 sinon
    int allow_SO;       // 1 si SO autorisé, 0 sinon
    int allow_NI;       // 1 si NI autorisé, 0 sinon
    int allow_MA;       // 1 si MA autorisé, 0 sinon
} Lecture;

// Lit le fichier crazy.cfg et remplit la structure Lecture
// Renvoie 1 si succès, 0 sinon
int lecture_load(Lecture* lec, const char* filename);

// Libère toute la mémoire dynamique associée à Lecture
void lecture_free(Lecture* lec);

/* ------------------------------------------------------------
   1) Lire UNE ligne complète en concaténant des blocs fgets.
      -> renvoie une chaine allouée (à free)
      -> renvoie NULL si on n'a rien à lire (EOF)
   ------------------------------------------------------------ */
static char* read_line_concat(FILE* f);

/* ------------------------------------------------------------
   2) Outils pour gérer un tableau dynamique de mots (char**)
   ------------------------------------------------------------ */
static void free_words(char** words, int n);

// Découpe "line" en mots séparés par espaces/tabs
// -> alloue words + chaque mot
static int split_words(const char* line, char*** out_words, int* out_n);

/* ------------------------------------------------------------
   3) Gestion des ordres autorisés (flags 0/1)
   ------------------------------------------------------------ */
static void reset_orders(Lecture* lec);

static int apply_order_token(Lecture* lec, const char* tok);

/* ------------------------------------------------------------
   4) Fonctions publiques : lecture_load / lecture_free
   ------------------------------------------------------------ */
int lecture_load(Lecture* lec, const char* filename);