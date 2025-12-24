#pragma once
#include <stdio.h>

typedef struct {
    int* animaux;  // tableau dynamique (cap éléments)
    int taille;    // nb d'animaux dans la pile
    int cap;       // capacité max d'animaux
} Podium;

// Initialise une pile vide de capacité cap
int podium_init(Podium* p, int cap);

// Libère la mémoire de la pile
void podium_free(Podium* p);

// Empile (push) un animal. Renvoie 1 si OK, 0 si pile pleine.
int podium_push(Podium* p, int animal);

// D�pile (pop) le sommet. Met la valeur dans *animal. Renvoie 1 si OK, 0 si vide.
int podium_pop(Podium* p, int* animal);

// Affichage debug (bas -> haut) en nombres
void podium_print(const Podium* p);

// Rotation bas -> haut (utile pour NI/MA). Renvoie 1 si OK, 0 si <= 1 �l�ment.
int podium_bas_vers_haut(Podium* p);
#pragma once
