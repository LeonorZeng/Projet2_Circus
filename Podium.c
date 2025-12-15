#include "Podium.h"
#include <stdio.h>

void podium_init(Podium* p) {
    p->taille = 0;
}

int podium_push(Podium* p, int animal) {
    if (p->taille >= MAX_ANIMAUX)
        return 0;

    p->animaux[p->taille] = animal;
    p->taille++;
    return 1;
}

int podium_pop(Podium* p, int* animal) {
    if (p->taille == 0)
        return 0;

    p->taille--;
    *animal = p->animaux[p->taille];
    return 1;
}

void podium_print(Podium* p) {
    printf("Podium (bas -> haut): ");
    for (int i = 0; i < p->taille; i++) {
        printf("%d ", p->animaux[i]);
    }
    printf("\n");
}
