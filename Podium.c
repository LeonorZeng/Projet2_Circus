#include "Podium.h"
#include <stdlib.h>

int podium_init(Podium* p, int cap) {
    if (cap <= 0) return 0;

    p->animaux = (int*)malloc((size_t)cap * sizeof(int));
    if (p->animaux == NULL) return 0;

    p->taille = 0;
    p->cap = cap;
    return 1;
}

void podium_free(Podium* p) {
    if (!p) return;
    free(p->animaux);
    p->animaux = NULL;
    p->taille = 0;
    p->cap = 0;
}

int podium_push(Podium* p, int animal) {
    if (p->taille >= p->cap) return 0;     // pile pleine
    p->animaux[p->taille] = animal;        // on met au sommet
    p->taille++;                           // sommet monte
    return 1;
}

int podium_pop(Podium* p, int* animal) {
    if (p->taille <= 0) return 0;          // pile vide
    p->taille--;                           // on descend le sommet
    *animal = p->animaux[p->taille];       // on r�cup�re l'ancien sommet
    return 1;
}

void podium_print(const Podium* p) {
    printf("Podium (bas -> haut): ");
    for (int i = 0; i < p->taille; i++) {
        printf("%d ", p->animaux[i]);
    }
    printf("\n");
}

int podium_bas_vers_haut(Podium* p) {
    if (p->taille <= 1) return 0;

    int bas = p->animaux[0];

    for (int i = 0; i < p->taille - 1; i++) {
        p->animaux[i] = p->animaux[i + 1];
    }

    p->animaux[p->taille - 1] = bas;
    return 1;
}
