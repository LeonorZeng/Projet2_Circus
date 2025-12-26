#pragma once

#include "Podium.h"
#include "Lecture.h"

typedef struct {
    Podium bleu;
    Podium rouge;
} Jeu;

// Initialisation / libération
void jeu_init(Jeu* j, const Lecture* lec);
void jeu_free(Jeu* j);

// Ordres
int ordre_KI(Jeu* j, const Lecture* lec);
int ordre_LO(Jeu* j, const Lecture* lec);
int ordre_SO(Jeu* j, const Lecture* lec);
int ordre_NI(Jeu* j, const Lecture* lec);
int ordre_MA(Jeu* j, const Lecture* lec);

// Applique une séquence collée : "KILOSONIMA"
// Renvoie 1 si toute la séquence réussit, 0 sinon
int jeu_appliquer_sequence(Jeu* j, const Lecture* lec, const char* seq);

void jeu_print(const Jeu* j, const Lecture* lec);

static int max_int(int a, int b);

static int max_name_len(const Lecture* lec);

static void print_spaces(int n);

static void print_center(const char* s, int width);

static void print_blank(int width);

static void print_dashes(int width);

static const char* name_from_id(const Lecture* lec, int id);

