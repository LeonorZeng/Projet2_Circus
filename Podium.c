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
	printf("Podium push %d\n", animal);
    return ajouter(p, animal);
}

int podium_pop(Podium* p) {
    assert(p != NULL);
    assert(p->nbElements > 0);              // il faut au moins 1 élément

    int idx = p->nbElements - 1;            // dernier index valide
    int animal = obtenir(p, idx);
    supprimer(p, idx);
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
    if (p->nbElements <= 1) 
        return 0;

    int bas = p->elements[0];

    for (int i = 0; i < p->nbElements - 1; i++) {
        p->elements[i] = p->elements[i + 1];
    }

    p->elements[p->nbElements - 1] = bas;
    return 1;
}

int podium_clone(Podium* dst, const Podium* src) {
    // On reste cohérent avec l'API Podium
    if (!initPodium(dst, src->capacite))
        return 0;

    for (int i = 0; i < src->nbElements; i++) {
        ItemV it = obtenir(src, i);
        if (!ajouter(dst, it)) {
            podium_free(dst);
            return 0;
        }
    }
    return 1;
}

int podium_equals(const Podium* a, const Podium* b) {
    if (a->nbElements != b->nbElements)
        return 0;

    for (int i = 0; i < a->nbElements; i++) {
        if (obtenir(a, i) != obtenir(b, i)) return 0;
    }
    return 1;
}

