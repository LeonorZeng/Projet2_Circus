#include "Lecture.h"

#include <stdio.h>   // fopen, fgets, fclose, printf
#include <stdlib.h>  // calloc, malloc, realloc, free
#include <string.h>  // strlen, strcat, strcmp
#include <assert.h>  // assert

// Quelques règlages Visual (comme dans l'exemple prof)
#pragma warning (disable : 4996)
#pragma warning (disable : 6308 28183)

static char* read_line_concat(FILE* f) {
    enum { BUFFER_SIZE = 10 };      // volontairement petit comme dans l'exemple
    char buffer[BUFFER_SIZE];

    // On accumule la ligne complète dans "line"
    char* line = (char*)calloc(1, 1);   // chaine vide ""
    if (line == NULL) return NULL;

    char* s = fgets(buffer, BUFFER_SIZE, f);
    if (s == NULL) {   // End-Of-File directe
        free(line);
        return NULL;
    }

    while (s != NULL) {
        size_t taille = strlen(buffer);
        assert(taille != 0);

        if (buffer[taille - 1] == '\n') {
            // fin de ligne lue : on retire '\n'
            buffer[taille - 1] = '\0';

            // si le morceau n'est pas vide, on concat�ne
            if (taille > 1) {
                // On agrandit line pour ajouter buffer
                line = (char*)realloc(line, strlen(line) + taille);
                strcat(line, buffer);
            }
            return line; // ligne complète pr�te
        }
        else {
            // pas fin de ligne : on concatène et on continue
            line = (char*)realloc(line, strlen(line) + taille + 1);
            strcat(line, buffer);
        }

        s = fgets(buffer, BUFFER_SIZE, f);
    }

    // Si le fichier se termine sans '\n'
    if (strlen(line) != 0) return line;

    free(line);
    return NULL;
}

static void free_words(char** words, int n) {
    if (!words) return;
    for (int i = 0; i < n; i++) free(words[i]);
    free(words);
}

static char* dup_word(const char* start, int len) {
    char* w = (char*)malloc((size_t)len + 1);
    if (!w) return NULL;
    for (int i = 0; i < len; i++) w[i] = start[i];
    w[len] = '\0';
    return w;
}

static int split_words(const char* line, char*** out_words, int* out_n) {
    *out_words = NULL;
    *out_n = 0;

    char** words = NULL;
    int n = 0;

    int i = 0;
    while (line[i] != '\0') {
        // sauter espaces/tabs
        while (line[i] == ' ' || line[i] == '\t') i++;
        if (line[i] == '\0') break;

        int start = i;
        while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') i++;
        int len = i - start;

        char* w = dup_word(line + start, len);
        if (!w) {
            free_words(words, n);
            return 0;
        }

        char** tmp = (char**)realloc(words, (size_t)(n + 1) * sizeof(char*));
        if (!tmp) {
            free(w);
            free_words(words, n);
            return 0;
        }
        words = tmp;
        words[n] = w;
        n++;
    }

    *out_words = words;
    *out_n = n;
    return 1;
}

static void reset_orders(Lecture* lec) {
    lec->allow_KI = 0;
    lec->allow_LO = 0;
    lec->allow_SO = 0;
    lec->allow_NI = 0;
    lec->allow_MA = 0;
}

static int apply_order_token(Lecture* lec, const char* tok) {
    if (strcmp(tok, "KI") == 0) lec->allow_KI = 1;
    else if (strcmp(tok, "LO") == 0) lec->allow_LO = 1;
    else if (strcmp(tok, "SO") == 0) lec->allow_SO = 1;
    else if (strcmp(tok, "NI") == 0) lec->allow_NI = 1;
    else if (strcmp(tok, "MA") == 0) lec->allow_MA = 1;
    else return 0; // ordre inconnu
    return 1;
}

int lecture_load(Lecture* lec, const char* filename) {
    // init propre
    lec->animaux = NULL;
    lec->n_animaux = 0;
    reset_orders(lec);

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("lecture_load: fichier non accessible (%s)\n", filename);
        return 0;
    }

    // Ligne 1 : animaux
    char* line_animaux = read_line_concat(f);
    if (line_animaux == NULL) {
        printf("lecture_load: ligne animaux manquante\n");
        fclose(f);
        return 0;
    }

    // Ligne 2 : ordres
    char* line_ordres = read_line_concat(f);
    if (line_ordres == NULL) {
        printf("lecture_load: ligne ordres manquante\n");
        free(line_animaux);
        fclose(f);
        return 0;
    }

    // D�couper animaux en mots
    char** animals = NULL;
    int n_animals = 0;
    if (!split_words(line_animaux, &animals, &n_animals)) {
        printf("lecture_load: erreur split animaux\n");
        free(line_animaux);
        free(line_ordres);
        fclose(f);
        return 0;
    }

    if (n_animals < 3) {
        printf("lecture_load: il faut au moins 3 animaux\n");
        free_words(animals, n_animals);
        free(line_animaux);
        free(line_ordres);
        fclose(f);
        return 0;
    }

    // D�couper ordres en mots
    char** orders = NULL;
    int n_orders = 0;
    if (!split_words(line_ordres, &orders, &n_orders)) {
        printf("lecture_load: erreur split ordres\n");
        free_words(animals, n_animals);
        free(line_animaux);
        free(line_ordres);
        fclose(f);
        return 0;
    }

    // Activer les flags d'ordres
    for (int i = 0; i < n_orders; i++) {
        if (!apply_order_token(lec, orders[i])) {
            printf("lecture_load: ordre inconnu '%s'\n", orders[i]);
            free_words(orders, n_orders);
            free_words(animals, n_animals);
            free(line_animaux);
            free(line_ordres);
            fclose(f);
            return 0;
        }
    }

    // Stocker dans la structure Lecture
    lec->animaux = animals;
    lec->n_animaux = n_animals;

    // Nettoyage
    free_words(orders, n_orders);
    free(line_animaux);
    free(line_ordres);
    fclose(f);

    return 1;
}

void lecture_free(Lecture* lec) {
    if (!lec) return;

    if (lec->animaux != NULL) {
        for (int i = 0; i < lec->n_animaux; i++) {
            free(lec->animaux[i]);
        }
        free(lec->animaux);
    }

    lec->animaux = NULL;
    lec->n_animaux = 0;
    reset_orders(lec);
}
