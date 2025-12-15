#pragma once

#include "Animaux.h"
#include "Podium.h"



typedef struct {
    Podium bleu;
    Podium rouge;
} Jeu;

void jeu_init(Jeu* j);
void jeu_print(Jeu* j);


int ordre_KI(Jeu* j);
int ordre_LO(Jeu* j);
int ordre_SO(Jeu* j);
int ordre_NI(Jeu* j);
int ordre_MA(Jeu* j);



