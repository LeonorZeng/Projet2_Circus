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
