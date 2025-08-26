#ifndef MEILLEUR_SCORE_H
#define MEILLEUR_SCORE_H

#include <SDL/SDL.h>
#include "header.h"

#define MAX_PLAYERS 10

typedef struct {
    char name[256];
    int score;
} PlayerScore;

// Fonctions de gestion de score
void sauvegarderScores(PlayerScore scores[], int nb);
void sauvegarderScoresTxt(PlayerScore scores[], int nb);
int chargerScores(PlayerScore scores[], int max);
void trierScores(PlayerScore scores[], int nb);

// Fonction principale du menu "meilleur score"
int meilleur(joueur j);

#endif // MEILLEUR_SCORE_H

