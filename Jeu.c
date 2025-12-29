#include "jeu.h"
#include <string.h>
#include <stdio.h>

/* ===================== HELPERS AFFICHAGE (static) ===================== */

static int max_int(int a, int b) { return (a > b) ? a : b; }

static int max_name_len(const Lecture* lec) {
    int m = 0;
    for (int i = 0; i < lec->n_animaux; i++) {
        int l = (int)strlen(lec->animaux[i]);
        if (l > m) m = l;
    }
    return m;
}

static void print_spaces(int n) { for (int i = 0; i < n; i++) putchar(' '); }

static void print_center(const char* s, int width) {
    int len = (int)strlen(s);
    int left = (width - len) / 2;
    int right = width - len - left;
    print_spaces(left);
    fputs(s, stdout);
    print_spaces(right);
}

static void print_blank(int width) { print_spaces(width); }

static void print_dashes(int width) { print_center("----", width); }

static const char* name_from_id(const Lecture* lec, int id) {
    if (id < 0 || id >= lec->n_animaux) return "";
    return lec->animaux[id];
}

/* ===================== CLONE / EQUALS (static + publics) ===================== */

static int podium_clone(Podium* dst, const Podium* src) {
    // On reste cohérent avec l'API Podium
    if (!initPodium(dst, src->capacite)) return 0;

    for (int i = 0; i < src->nbElements; i++) {
        ItemV it = obtenir(src, i);
        if (!ajouter(dst, it)) {
            podium_free(dst);
            return 0;
        }
    }
    return 1;
}

static int podium_equals(const Podium* a, const Podium* b) {
    if (a->nbElements != b->nbElements) return 0;

    for (int i = 0; i < a->nbElements; i++) {
        if (obtenir(a, i) != obtenir(b, i)) return 0;
    }
    return 1;
}

int jeu_clone(Jeu* dst, const Jeu* src) {
    if (!podium_clone(&dst->bleu, &src->bleu)) return 0;

    if (!podium_clone(&dst->rouge, &src->rouge)) {
        podium_free(&dst->bleu);
        return 0;
    }
    return 1;
}

int jeu_equals(const Jeu* a, const Jeu* b) {
    return podium_equals(&a->bleu, &b->bleu) &&
        podium_equals(&a->rouge, &b->rouge);
}

int sequence_reussit(const Jeu* depart, const Jeu* objectif,
    const Lecture* lec, const char* seq) {
    Jeu tmp;
    if (!jeu_clone(&tmp, depart)) return 0;

    int ok = jeu_appliquer_sequence(&tmp, lec, seq) &&
        jeu_equals(&tmp, objectif);

    jeu_free(&tmp);
    return ok;
}

/* ===================== INIT / FREE ===================== */

int jeu_init(Jeu* j, const Lecture* lec) {
    if (!initPodium(&j->bleu, lec->n_animaux)) return 0;

    if (!initPodium(&j->rouge, lec->n_animaux)) {
        podium_free(&j->bleu);
        return 0;
    }
    return 1;
}

void jeu_free(Jeu* j) {
    podium_free(&j->bleu);
    podium_free(&j->rouge);
}

/* ===================== ORDRES ===================== */

int ordre_KI(Jeu* j, const Lecture* lec) {
    if (!lec->allow_KI) return 0;
    if (j->bleu.nbElements == 0) return 0;

    int a = podium_pop(&j->bleu);
    return podium_push(&j->rouge, a);
}

int ordre_LO(Jeu* j, const Lecture* lec) {
    if (!lec->allow_LO) return 0;
    if (j->rouge.nbElements == 0) return 0;

    int a = podium_pop(&j->rouge);
    return podium_push(&j->bleu, a);
}

int ordre_SO(Jeu* j, const Lecture* lec) {
    if (!lec->allow_SO) return 0;
    if (j->bleu.nbElements == 0) return 0;
    if (j->rouge.nbElements == 0) return 0;

    int ab = podium_pop(&j->bleu);
    int ar = podium_pop(&j->rouge);

    // On suppose que push ne peut pas échouer ici car capacité = n_animaux
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

/* ===================== SEQUENCE ===================== */

int jeu_appliquer_sequence(Jeu* j, const Lecture* lec, const char* seq) {
    if (seq == NULL) return 0;

    size_t len = strlen(seq);
    if (len % 2 != 0) return 0;

    for (size_t i = 0; i < len; i += 2) {
        char ordre[3] = { seq[i], seq[i + 1], '\0' };

        int ok = 0;
        if (strcmp(ordre, "KI") == 0) ok = ordre_KI(j, lec);
        else if (strcmp(ordre, "LO") == 0) ok = ordre_LO(j, lec);
        else if (strcmp(ordre, "SO") == 0) ok = ordre_SO(j, lec);
        else if (strcmp(ordre, "NI") == 0) ok = ordre_NI(j, lec);
        else if (strcmp(ordre, "MA") == 0) ok = ordre_MA(j, lec);
        else return 0; // ordre inconnu

        if (!ok) return 0;
    }
    return 1;
}

/* ===================== AFFICHAGE STRICT ===================== */

void jeu_print(const Jeu* j, const Lecture* lec) {
    int w = max_name_len(lec);
    w = max_int(w, 5); // "ROUGE"
    w = max_int(w, 4); // "----"

    int hb = j->bleu.nbElements;
    int hr = j->rouge.nbElements;
    int h = max_int(hb, hr);

    for (int row = h - 1; row >= 0; row--) {
        if (row < hb) {
            int id = j->bleu.elements[row];
            print_center(name_from_id(lec, id), w);
        }
        else {
            print_blank(w);
        }

        print_spaces(2);

        if (row < hr) {
            int id = j->rouge.elements[row];
            print_center(name_from_id(lec, id), w);
        }
        else {
            print_blank(w);
        }

        putchar('\n');
    }

    print_dashes(w);
    print_spaces(2);
    print_dashes(w);
    putchar('\n');

    print_center("BLEU", w);
    print_spaces(2);
    print_center("ROUGE", w);
    putchar('\n');
}
