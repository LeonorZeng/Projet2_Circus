#pragma once
#include <stdio.h>

typedef struct {
    char** animaux;
    int n_animaux;

    int allow_KI;
    int allow_LO;
    int allow_SO;
    int allow_NI;
    int allow_MA;
} Lecture;

// Lit le fichier crazy.cfg et remplit la structure Lecture
// Renvoie 1 si succès, 0 sinon
int lecture_load(Lecture* lec, const char* filename);

// Libère toute la mémoire dynamique associée à Lecture
void lecture_free(Lecture* lec);

const char* name_from_id(const Lecture* lec, int id);

// Fonctions internes (utilisées uniquement dans Lecture.c)
static char* read_line_concat(FILE* f);
static void free_words(char** words, int n);
static char* dup_word(const char* start, int len);
static int split_words(const char* line, char*** out_words, int* out_n);
static void reset_orders(Lecture* lec);
static int apply_order_token(Lecture* lec, const char* tok);

