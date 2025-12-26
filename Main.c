#include "jeu.h"

const char* const FILE_NAME = "crazy.cfg";

int main(int argc, const char* argv[]) {
	printf("%d parametre(s) recu(s) sur la ligne de commande\n", argc - 1);
	for (int i = 1; i < argc; ++i)
		printf("%s\n", argv[i]);
	printf("\n");
}