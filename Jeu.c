#include "jeu.h"
#include <string.h>
#include <stdio.h>

/* ===================== INIT / FREE ===================== */

int jeu_init(Jeu* j, const Lecture* lec) {
    if (!initPodium(&j->bleu, lec->n_animaux)) 
        return 0;

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

/* Convertit une Card vers un Jeu (podiums remplis bas->haut) */
int jeu_from_card(Jeu* j, const Lecture* lec, const Card* c) {
    /* initPodium(cap = n_animaux) */
    if (!initPodium(&j->bleu, lec->n_animaux)) return 0;
    if (!initPodium(&j->rouge, lec->n_animaux)) {
        podium_free(&j->bleu);
        return 0;
    }

    for (int i = 0; i < c->nb_bleu; i++) {
        if (!podium_push(&j->bleu, c->bleu[i])) return 0;
    }
    for (int i = 0; i < c->nb_rouge; i++) {
        if (!podium_push(&j->rouge, c->rouge[i])) return 0;
    }
    return 1;
}

/* ===================== ORDRES ===================== */

int ordre_KI(Jeu* j, const Lecture* lec) {
    if (!lec->allow_KI) {
		printf("Ordre non autoriser\n");
        return 0;
    }
    if (j->bleu.nbElements == 0)
        return 0;

    int a = podium_pop(&j->bleu);
    return podium_push(&j->rouge, a);
}

int ordre_LO(Jeu* j, const Lecture* lec) {
    if (!lec->allow_LO){
        printf("Ordre non autoriser\n");
        return 0;
    }
    if (j->rouge.nbElements == 0) 
        return 0;

    int a = podium_pop(&j->rouge);
    return podium_push(&j->bleu, a);
}

int ordre_SO(Jeu* j, const Lecture* lec) {
    if (!lec->allow_SO) {
        printf("Ordre non autoriser\n");
        return 0;
    }
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
    if (!lec->allow_NI) {
        printf("Ordre non autoriser\n");
        return 0;
    }
    return podium_bas_vers_haut(&j->bleu);
}

int ordre_MA(Jeu* j, const Lecture* lec) {
    if (!lec->allow_MA) {
        printf("Ordre non autoriser\n");
        return 0;
    }
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
        if (strcmp(ordre, "KI") == 0) 
            ok = ordre_KI(j, lec);
        else if (strcmp(ordre, "LO") == 0) 
            ok = ordre_LO(j, lec);
        else if (strcmp(ordre, "SO") == 0) 
            ok = ordre_SO(j, lec);
        else if (strcmp(ordre, "NI") == 0) 
            ok = ordre_NI(j, lec);
        else if (strcmp(ordre, "MA") == 0) 
            ok = ordre_MA(j, lec);
        else return 0; // ordre inconnu

        if (!ok) return 0;
    }
    return 1;
}

/* ===================== AFFICHAGE ===================== */
int max_name_len(const Lecture* lec) {
    int m = 0;
    for (int i = 0; i < lec->n_animaux; i++) {
        m = (m > (int)strlen(lec->animaux[i])) ? m : (int)strlen(lec->animaux[i]);
    }
    return m;
}

void print_spaces(int n) {
    for (int i = 0; i < n; i++)
        putchar(' ');
}

void print_left(const char* s, int width) {
    int len = (int)strlen(s);
    int right = width - len;
    fputs(s, stdout);
    print_spaces(right);
}

void largeur(int* wb, int* wr, const Jeu* jeu, const Lecture* lec) {
    *wb = 4;
    *wr = 5;
    for (int i = 0; i < jeu->bleu.nbElements; ++i)
        *wb = (*wb > strlen(name_from_id(lec, jeu->bleu.elements[i]))) ? *wb : strlen(name_from_id(lec, jeu->bleu.elements[i]));

    for (int i = 0; i < jeu->rouge.nbElements; ++i)
        *wr = (*wr > strlen(name_from_id(lec, jeu->rouge.elements[i]))) ? *wr : strlen(name_from_id(lec, jeu->rouge.elements[i]));

}

/* Affiche 2 jeux côte à côte selon le format de l’énoncé : */
void affichage(const Jeu* depart, const Jeu* objectif, const Lecture* lec) {
    if (!depart || !objectif || !lec)
        return;

    int wbD, wrD, wbO, wrO; ///< ce sont les largueurs des podiums pour depart (D) et objectif (O)
    largeur(&wbD, &wrD, depart, lec);
    largeur(&wbO, &wrO, objectif, lec);

    int h1 = depart->bleu.nbElements > depart->rouge.nbElements ? depart->bleu.nbElements : depart->rouge.nbElements;

    int h2 = objectif->bleu.nbElements > objectif->rouge.nbElements ? objectif->bleu.nbElements : objectif->rouge.nbElements;

    int h = h1 > h2 ? h1 : h2;

    /* lignes animaux */
    for (int row = h - 1; row >= 0; row--) {

        /* depart BLEU */
        if (row < depart->bleu.nbElements)
            print_left(name_from_id(lec, depart->bleu.elements[row]), wbD);
        else
            print_spaces(wbD);

        print_spaces(2);

        /* depart ROUGE */
        if (row < depart->rouge.nbElements)
            print_left(name_from_id(lec, depart->rouge.elements[row]), wrD);
        else
            print_spaces(wrD);

        // espace entre départ et objectif est fixe à 7 espaces
        print_spaces(7);


        /* objectif BLEU */
        if (row < objectif->bleu.nbElements)
            print_left(name_from_id(lec, objectif->bleu.elements[row]), wbO);
        else
            print_spaces(wbO);

        print_spaces(2);

        /* objectif ROUGE */
        if (row < objectif->rouge.nbElements)
            print_left(name_from_id(lec, objectif->rouge.elements[row]), wrO);
        else
            print_spaces(wrO);

        putchar('\n');
    }


    /* ligne dashes avec ==> au milieu */
    print_left("----", wbD);
    print_spaces(2);
    print_left("----", wrD);

    printf("  ==>  ");

    print_left("----", wbO);
    print_spaces(2);
    print_left("----", wrO);
    putchar('\n');

    /* labels */
    print_left("BLEU", wbD);
    print_spaces(2);
    print_left("ROUGE", wrD);

    // espace entre départ et objectif est fixe à 7 espaces
    print_spaces(7);

    print_left("BLEU", wbO);
    print_spaces(2);
    print_left("ROUGE", wrO);
    putchar('\n');
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
    if (!jeu_clone(&tmp, depart)){
        printf("Erreur memoire.\n");
        return 0;
    }
        
    int ok = jeu_appliquer_sequence(&tmp, lec, seq) && jeu_equals(&tmp, objectif);
    jeu_free(&tmp);

    return ok;
}

void test_jeu(const Jeu* depart, const Jeu* objectif,
    const Lecture* lec, const char* seq) {
        printf("Avant:\n");
    affichage(depart, objectif, lec);
    if (sequence_reussit(depart,objectif,
        lec, seq)){
        printf("Apres:\n");
        affichage(depart, objectif, lec);
    }
    else{
        printf("Echec de l'application de la sequence %s\n", seq);
	}
}