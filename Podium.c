#include "Podium.h"
#include <stdlib.h>

int podium_init(Podium* p, int cap) {
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
