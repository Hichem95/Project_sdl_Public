#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>  // Si utilisé pour l'audio
#include <stdbool.h>

// Structure pour gérer le fond d'écran
typedef struct {
    SDL_Surface* bg1;       // Surface pour le premier fond d'écran
    SDL_Surface* bg2;       // Surface pour le second fond d'écran
    SDL_Rect camera1;       // Position et dimensions de la caméra pour le joueur 1
    SDL_Rect camera2;       // Position et dimensions de la caméra pour le joueur 2
    SDL_Rect cameraSolo;    // Position et dimensions de la caméra pour le mode solo
    SDL_Rect posEcran1;     // Position d'affichage pour le joueur 1
    SDL_Rect posEcran2;     // Position d'affichage pour le joueur 2
} Background;

// Fonction pour afficher le temps de jeu écoulé depuis `tempsDebut`
Uint32 afficherTempsJeu(Uint32 tempsDebut);

// Fonction d'initialisation des fonds d'écran avec les chemins des images
void initialiserBackground(Background* bg, const char* cheminImage1, const char* cheminImage2);

// Fonctions de scrolling pour gérer le déplacement des caméras
void updateBackground(Background *bg);  // Met à jour l'arrière-plan si nécessaire (fonction manquante dans le code partagé)
void scrollingCamera1(Background* bg, int direction, int dx, int dy); // Déplacement de la caméra 1
void scrollingCamera2(Background* bg, int direction, int dx, int dy); // Déplacement de la caméra 2
void scrollingCameraSolo(Background* bg, int direction, int dx, int dy); // Déplacement pour le mode solo

// Fonctions pour afficher les fonds d'écran et gérer l'interface utilisateur
void splitScreen(SDL_Surface* screen, Background* bg); // Affichage en mode split screen
void displaySingleBackground(SDL_Surface* screen, Background* bg); // Affichage du fond d'écran en mode solo
void afficherGuideTouches(SDL_Surface* screen, SDL_Event* event, bool* guideVisible, int* guideStep); // Affichage du guide des touches




#endif // HEADER_H

