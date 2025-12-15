#include "Jeu.h"
#include <stdio.h>

void jeu_init(Jeu* j) {
    podium_init(&j->bleu);
    podium_init(&j->rouge);
}

void jeu_print(Jeu* j) {
    printf("=== JEU ===\n");
    printf("BLEU  : ");
    podium_print(&j->bleu);
    printf("ROUGE : ");
    podium_print(&j->rouge);
}

int ordre_KI(Jeu* j) {
    int animal;

    if (!podium_pop(&j->bleu, &animal)) {
        return 0;
    }
    podium_push(&j->rouge, animal);
    return 1;
}

int ordre_LO(Jeu* j) {
    int animal;

    if (!podium_pop(&j->rouge, &animal)) {
        return 0;
    }
    podium_push(&j->bleu, animal);
    return 1;
}

int ordre_SO(Jeu* j) {
    int ani_bleu;
    int ani_rouge;

    // Il faut au moins un animal sur chaque podium
    if (!podium_pop(&j->bleu, &ani_bleu))
        return 0;

    if (!podium_pop(&j->rouge, &ani_rouge)) {
        podium_push(&j->bleu, ani_bleu);
        return 0;
    }

    // échange des sommets
    podium_push(&j->bleu, ani_rouge);
    podium_push(&j->rouge, ani_bleu);

    return 1;
}

int ordre_NI(Jeu* j) {
    int animal_bas;

    // impossible s'il y a 0 ou 1 animal
    if (j->bleu.taille <= 1)
        return 0;

    // animal du bas
    animal_bas = j->bleu.animaux[0];

    // on décale tous les animaux vers le bas
    for (int i = 0; i < j->bleu.taille - 1; i++) {
        j->bleu.animaux[i] = j->bleu.animaux[i + 1];
    }

    // on met l'animal du bas en haut
    j->bleu.animaux[j->bleu.taille - 1] = animal_bas;

    return 1;
}


int ordre_MA(Jeu* j) {
    int animal_bas;

    // impossible s'il y a 0 ou 1 animal
    if (j->rouge.taille <= 1)
        return 0;

    // animal du bas
    animal_bas = j->rouge.animaux[0];

    // on décale tous les animaux vers le bas
    for (int i = 0; i < j->rouge.taille - 1; i++) {
        j->rouge.animaux[i] = j->rouge.animaux[i + 1];
    }

    // on met l'animal du bas en haut
    j->rouge.animaux[j->rouge.taille - 1] = animal_bas;

    return 1;
}
