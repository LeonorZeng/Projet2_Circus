#pragma once

typedef struct {
    char** animaux;     // tableau dynamique de cha�nes (noms des animaux)
    int n_animaux;      // nombre d�animaux (>= 3)

    int allow_KI;       // 1 si KI autoris�, 0 sinon
    int allow_LO;       // 1 si LO autoris�, 0 sinon
    int allow_SO;       // 1 si SO autoris�, 0 sinon
    int allow_NI;       // 1 si NI autoris�, 0 sinon
    int allow_MA;       // 1 si MA autoris�, 0 sinon
} Lecture;

// Lit le fichier crazy.cfg et remplit la structure Lecture
// Renvoie 1 si succ�s, 0 sinon
int lecture_load(Lecture* lec, const char* filename);

// Lib�re toute la m�moire dynamique associ�e � Lecture
void lecture_free(Lecture* lec);
#pragma once
#pragma once
