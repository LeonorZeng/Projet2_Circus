#pragma once

#include "Podium.h"
#include "Lecture.h"

typedef struct {
    Podium bleu;
    Podium rouge;
} Jeu;

// Initialisation / lib�ration
int jeu_init(Jeu* j, const Lecture* lec);
void jeu_free(Jeu* j);

// Ordres
int ordre_KI(Jeu* j, const Lecture* lec);
int ordre_LO(Jeu* j, const Lecture* lec);
int ordre_SO(Jeu* j, const Lecture* lec);
int ordre_NI(Jeu* j, const Lecture* lec);
int ordre_MA(Jeu* j, const Lecture* lec);

// Applique une s�quence coll�e : "KILOSONIMA"
// Renvoie 1 si toute la s�quence r�ussit, 0 sinon
int jeu_appliquer_sequence(Jeu* j, const Lecture* lec, const char* seq);

void jeu_print(const Jeu* j, const Lecture* lec);
