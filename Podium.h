#pragma once

#include "Animaux.h"

#define MAX_ANIMAUX 3

typedef struct {
    int animaux[MAX_ANIMAUX];
    int taille;
} Podium;

void podium_init(Podium* p);
int podium_push(Podium* p, int animal);
int podium_pop(Podium* p, int* animal);
void podium_print(Podium* p);

