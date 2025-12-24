#include "Jeu.h"
#include <string.h> // strlen
#include <stdio.h>



// --------------------
// Initialisation / fin
// --------------------
int jeu_init(Jeu* j, const Lecture* lec) {
    if (!podium_init(&j->bleu, lec->n_animaux)) return 0;
    if (!podium_init(&j->rouge, lec->n_animaux)) {
        podium_free(&j->bleu);
        return 0;
    }
    return 1;
}

void jeu_free(Jeu* j) {
    podium_free(&j->bleu);
    podium_free(&j->rouge);
}

// --------------------
// Ordres
// --------------------
int ordre_KI(Jeu* j, const Lecture* lec) {
    if (!lec->allow_KI) return 0;

    int a;
    if (!podium_pop(&j->bleu, &a)) return 0;
    podium_push(&j->rouge, a);
    return 1;
}

int ordre_LO(Jeu* j, const Lecture* lec) {
    if (!lec->allow_LO) return 0;

    int a;
    if (!podium_pop(&j->rouge, &a)) return 0;
    podium_push(&j->bleu, a);
    return 1;
}

int ordre_SO(Jeu* j, const Lecture* lec) {
    if (!lec->allow_SO) return 0;

    int ab, ar;
    if (!podium_pop(&j->bleu, &ab)) return 0;
    if (!podium_pop(&j->rouge, &ar)) {
        podium_push(&j->bleu, ab);
        return 0;
    }

    podium_push(&j->bleu, ar);
    podium_push(&j->rouge, ab);
    return 1;
}

int ordre_NI(Jeu* j, const Lecture* lec) {
    if (!lec->allow_NI) return 0;
    return podium_bas_vers_haut(&j->bleu);
}

int ordre_MA(Jeu* j, const Lecture* lec) {
    if (!lec->allow_MA) return 0;
    return podium_bas_vers_haut(&j->rouge);
}


int jeu_appliquer_sequence(Jeu* j, const Lecture* lec, const char* seq) {
    if (seq == NULL) return 0;

    size_t len = strlen(seq);

    if (len % 2 != 0)
        return 0;

    for (size_t i = 0; i < len; i += 2) {
        char ordre[3];
        ordre[0] = seq[i];
        ordre[1] = seq[i + 1];
        ordre[2] = '\0';

        int ok;

        if (strcmp(ordre, "KI") == 0) ok = ordre_KI(j, lec);
        else if (strcmp(ordre, "LO") == 0) ok = ordre_LO(j, lec);
        else if (strcmp(ordre, "SO") == 0) ok = ordre_SO(j, lec);
        else if (strcmp(ordre, "NI") == 0) ok = ordre_NI(j, lec);
        else if (strcmp(ordre, "MA") == 0) ok = ordre_MA(j, lec);
        else return 0;

        if (!ok) return 0;
    }

    return 1;
}


static int max_int(int a, int b) { return (a > b) ? a : b; }

static int max_name_len(const Lecture* lec) {
    int m = 0;
    for (int i = 0; i < lec->n_animaux; i++) {
        int l = (int)strlen(lec->animaux[i]);
        if (l > m) m = l;
    }
    return m;
}

static void print_spaces(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

static void print_center(const char* s, int width) {
    int len = (int)strlen(s);
    int left = (width - len) / 2;
    int right = width - len - left;
    print_spaces(left);
    fputs(s, stdout);
    print_spaces(right);
}

static void print_blank(int width) {
    print_spaces(width);
}

// Affiche une ligne "----" centr�e dans la largeur de colonne
static void print_dashes(int width) {
    const char* d = "----";
    print_center(d, width);
}

static const char* name_from_id(const Lecture* lec, int id) {
    if (id < 0 || id >= lec->n_animaux) return "";
    return lec->animaux[id];
}

void jeu_print(const Jeu* j, const Lecture* lec) {
    // largeur de colonne = max(longueur des noms, longueur de "ROUGE", 4)
    int w = max_name_len(lec);
    w = max_int(w, 5); // "ROUGE" = 5
    w = max_int(w, 4); // "----" = 4

    int hb = j->bleu.taille;
    int hr = j->rouge.taille;
    int h = max_int(hb, hr);

    // On affiche de haut en bas
    for (int row = h - 1; row >= 0; row--) {
        // BLEU
        if (row < hb) {
            const char* nb = name_from_id(lec, j->bleu.animaux[row]);
            print_center(nb, w);
        }
        else {
            print_blank(w);
        }

        // espace entre colonnes (comme sur l'exemple)
        print_spaces(2);

        // ROUGE
        if (row < hr) {
            const char* nr = name_from_id(lec, j->rouge.animaux[row]);
            print_center(nr, w);
        }
        else {
            print_blank(w);
        }

        putchar('\n');
    }

    // ligne ---- sous chaque colonne
    print_dashes(w);
    print_spaces(2);
    print_dashes(w);
    putchar('\n');

    // labels
    print_center("BLEU", w);
    print_spaces(2);
    print_center("ROUGE", w);
    putchar('\n');
}

