#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "podium.h"

int initPodium(Podium* p, int cap) {
    return initVecteur(p, cap);
}

void podium_free(Podium* p) {
    detruireVecteur(p);
}

int podium_push(Podium* p, int animal) {
    return ajouter(p, animal);
}

int podium_pop(Podium* p) {
    int animal;
    assert(p->nbElements >= 0);
    animal = obtenir(p, p->nbElements);
    supprimer(p, p->nbElements);
    return animal;
}

void podium_print(const Podium* p) {
    printf("Podium (bas -> haut): ");
    for (int i = 0; i < p->nbElements; i++) {
        printf("%d ", p->elements[i]);
    }
    printf("\n");
}

int podium_bas_vers_haut(Podium* p) {
    if (p->nbElements <= 1) return 0;

    int bas = p->elements[0];

    for (int i = 0; i < p->nbElements - 1; i++) {
        p->elements[i] = p->elements[i + 1];
    }

    p->elements[p->nbElements - 1] = bas;
    return 1;
}
