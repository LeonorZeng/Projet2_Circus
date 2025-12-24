#include <assert.h>
#include <stdlib.h>

#include "liste.h"

void initListe(Liste* li) {
	li->premier = NULL;
}

int estVide(const Liste* li) {
	return li->premier == NULL;
}

int inclure(Liste* li, ItemL it) {
	Maillon* m = (Maillon*)malloc(sizeof(Maillon));
	if (m == NULL)
		return 0;
	m->element = it;
	m->suivant = li->premier;
	li->premier = m;
	return 1;
}

ItemL premier(const Liste* li) {
	assert(!estVide(li));
	return li->premier->element;
}

void tronquer(Liste* li) {
	assert(!estVide(li));
	Maillon* m = li->premier;
	li->premier = m->suivant;
	free(m);
}

void detruireListe(Liste* li) {
	while (!estVide(li))
		tronquer(li);
}

ConstIt iterer(const Liste* li) {
	return li->premier;
}

int fini(const ConstIt* iter) {
	return *iter == NULL;
}

ItemL courant(const ConstIt* iter) {
	assert(!fini(iter));
	return (*iter)->element;
}

void suivant(ConstIt* iter) {
	assert(!fini(iter));
	*iter = (*iter)->suivant;
}

void initIterateur(Liste* li, Iterateur* iter) {
	iter->liste = li;
	iter->pred = NULL;
	iter->courant = li->premier;
}

int finiIt(const Iterateur* iter) {
	return iter->courant == NULL;
}

ItemL courantIt(const Iterateur* iter) {
	assert(!finiIt(iter));
	return iter->courant->element;
}

void suivantIt(Iterateur* iter) {
	assert(!finiIt(iter));
	iter->pred = iter->courant;
	iter->courant = iter->courant->suivant;
}

void changer(const Iterateur* iter, ItemL it) {
	assert(!finiIt(iter));
	iter->courant->element = it;
}

int inserer(Iterateur* iter, ItemL it) {
	Maillon* m = (Maillon*)malloc(sizeof(Maillon));
	if (m == NULL)
		return 0;
	m->element = it;
	m->suivant = iter->courant;
	if (iter->pred == NULL)
		iter->liste->premier = m;
	else
		iter->pred->suivant = m;
	iter->courant = m;
	return 1;
}

ItemL effacer(Iterateur* iter) {
	assert(!finiIt(iter));
	Maillon* m = iter->courant;
	if (iter->pred == NULL)
		iter->liste->premier = m->suivant;
	else
		iter->pred->suivant = m->suivant;
	iter->courant = m->suivant;
	ItemL it = m->element;
	free(m);
	return it;
}
