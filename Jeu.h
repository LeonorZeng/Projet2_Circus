#pragma once

#include "Podium.h"
#include "Lecture.h"

typedef struct {
    Podium bleu;
    Podium rouge;
} Jeu;

// Initialisation / libération
int  jeu_init(Jeu* j, const Lecture* lec);
void jeu_free(Jeu* j);

// Ordres
int ordre_KI(Jeu* j, const Lecture* lec);
int ordre_LO(Jeu* j, const Lecture* lec);
int ordre_SO(Jeu* j, const Lecture* lec);
int ordre_NI(Jeu* j, const Lecture* lec);
int ordre_MA(Jeu* j, const Lecture* lec);

// Applique une séquence collée : "KILOSONIMA"
int jeu_appliquer_sequence(Jeu* j, const Lecture* lec, const char* seq);

// Affichage
void jeu_print(const Jeu* j, const Lecture* lec);

// Copie / comparaison / validation
int jeu_clone(Jeu* dst, const Jeu* src);
int jeu_equals(const Jeu* a, const Jeu* b);

int sequence_reussit(const Jeu* depart, const Jeu* objectif,
    const Lecture* lec, const char* seq);
