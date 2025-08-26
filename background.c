#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <SDL/SDL_ttf.h>
#include "header.h"
#include "background.h"



// Fonction pour calculer le temps écoulé depuis le début du jeu
Uint32 afficherTempsJeu(Uint32 tempsDebut) {
    return (SDL_GetTicks() - tempsDebut) / 1000;
}

// Initialisation des paramètres du background
void initialiserBackground(Background* bg, const char* cheminImage1, const char* cheminImage2) {
    if (!bg) return;
    bg->bg1 = IMG_Load(cheminImage1);
    bg->bg2 = IMG_Load(cheminImage2);
    if (!bg->bg1 || !bg->bg2) {
        printf("Erreur chargement background : %s\n", IMG_GetError());
        return;
    }

    // Initialisation des caméras
    bg->camera1.x = 0;  bg->camera1.y = 300;  bg->camera1.w = 1200;  bg->camera1.h = 800 / 2;
    bg->camera2.x = 0;  bg->camera2.y = 300;  bg->camera2.w = 1200;  bg->camera2.h = 800 / 2;
    bg->cameraSolo.x = 0; bg->cameraSolo.y = 0; bg->cameraSolo.w = 1200; bg->cameraSolo.h = 800;
    bg->posEcran1.x = 0; bg->posEcran1.y = 0;
    bg->posEcran2.x = 0; bg->posEcran2.y = 800 / 2;
}

// Mise à jour des positions des caméras
void updateBackground(Background *bg) {
    if (!bg || !bg->bg1 || !bg->bg2) return;
    if (bg->camera1.x < 0) bg->camera1.x = 0;
    if (bg->camera1.x > bg->bg1->w - bg->camera1.w) bg->camera1.x = bg->bg1->w - bg->camera1.w;
    if (bg->camera1.y < 0) bg->camera1.y = 0;
    if (bg->camera1.y > bg->bg1->h - bg->camera1.h) bg->camera1.y = bg->bg1->h - bg->camera1.h;

    if (bg->camera2.x < 0) bg->camera2.x = 0;
    if (bg->camera2.x > bg->bg2->w - bg->camera2.w) bg->camera2.x = bg->bg2->w - bg->camera2.w;
    if (bg->camera2.y < 0) bg->camera2.y = 0;
    if (bg->camera2.y > bg->bg2->h - bg->camera2.h) bg->camera2.y = bg->bg2->h - bg->camera2.h;

    if (bg->cameraSolo.x < 0) bg->cameraSolo.x = 0;
    if (bg->cameraSolo.x > bg->bg1->w - bg->cameraSolo.w) bg->cameraSolo.x = bg->bg1->w - bg->cameraSolo.w;
    if (bg->cameraSolo.y < 0) bg->cameraSolo.y = 0;
    if (bg->cameraSolo.y > bg->bg1->h - bg->cameraSolo.h) bg->cameraSolo.y = bg->bg1->h - bg->cameraSolo.h;
}

// Fonction pour déplacer la caméra dans différentes directions
void scrollingCamera1(Background* bg, int direction, int dx, int dy) {
    if (!bg) return;
    switch (direction) {
        case 0: bg->camera1.x -= dx; break;
        case 1: bg->camera1.x += dx; break;
        case 2: bg->camera1.y -= dy; break;
        case 3: bg->camera1.y += dy; break;
    }
    updateBackground(bg);
}

void scrollingCamera2(Background* bg, int direction, int dx, int dy) {
    if (!bg) return;
    switch (direction) {
        case 0: bg->camera2.x -= dx; break;
        case 1: bg->camera2.x += dx; break;
        case 2: bg->camera2.y -= dy; break;
        case 3: bg->camera2.y += dy; break;
    }
    updateBackground(bg);
}

void scrollingCameraSolo(Background* bg, int direction, int dx, int dy) {
    if (!bg) return;
    switch (direction) {
        case 0: bg->cameraSolo.x -= dx; break;
        case 1: bg->cameraSolo.x += dx; break;
        case 2: bg->cameraSolo.y -= dy; break;
        case 3: bg->cameraSolo.y += dy; break;
    }
    updateBackground(bg);
}

// Rendu du mode split screen
void splitScreen(SDL_Surface* screen, Background* bg) {
    if (!bg || !bg->bg1 || !bg->bg2) return;
    SDL_Rect rect1 = bg->camera1;
    rect1.h = 800 / 2;
    SDL_BlitSurface(bg->bg1, &rect1, screen, &bg->posEcran1);

    SDL_Rect rect2 = bg->camera2;
   rect2.h = 400;

if (bg->camera2.y + rect2.h > bg->bg2->h) {
    rect2.y = bg->bg2->h - rect2.h;
} else {
    rect2.y = bg->camera2.y;
}

SDL_BlitSurface(bg->bg2, &rect2, screen, &bg->posEcran2);
}
// Affichage du fond en mode solo
void displaySingleBackground(SDL_Surface* screen, Background* bg) {
    SDL_Rect rect = bg->cameraSolo;
    SDL_BlitSurface(bg->bg1, &rect, screen, &bg->posEcran1);
}

// Fonction pour afficher le guide dynamique des touches
void afficherGuideTouches(SDL_Surface* screen, SDL_Event* event, bool* guideVisible, int* guideStep) {
    TTF_Font* font = TTF_OpenFont("alagard.ttf", 32);
    SDL_Color white = {255, 255, 255};
    SDL_Color shadow = {0, 0, 0};

    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return;
    }

    // Messages à afficher, chaque message correspondant à une touche spécifique
    const char* messages[] = {
        "Commande de jeux pour Joueur 1 : Flèche gauche.",
        "Commande de jeux pour Joueur 1 : Flèche droite.",
        "Commande de jeux pour Joueur 1 : Flèche haut.",
        "Commande de jeux pour Joueur 1 : Flèche bas.",
        "Commande de jeux pour Joueur 2 : Z.",
        "Commande de jeux pour Joueur 2 : Q.",
        "Commande de jeux pour Joueur 2 : S.",
        "Commande de jeux pour Joueur 2 : D.",
        "Appuyez sur ENTER pour fermer le guide ou F1 pour retrouver le guide."

    };

    // Position de départ
    SDL_Rect pos;
    pos.x = screen->w / 2;  // Centrer le texte horizontalement
    pos.y = screen->h - 150; // Position en bas de l'écran

    // Vérification des événements pour passer à l'étape suivante
    if (*guideVisible) {
        if (event->type == SDL_KEYDOWN) {
            // Étapes spécifiques pour chaque touche
            switch (*guideStep) {
                case 0: // Flèche gauche
                    if (event->key.keysym.sym == SDLK_LEFT) {
                        *guideStep = 1; // Passe à l'étape suivante
                    }
                    break;
                case 1: // Flèche droite
                    if (event->key.keysym.sym == SDLK_RIGHT) {
                        *guideStep = 2;
                    }
                    break;
                case 2: // Flèche haut
                    if (event->key.keysym.sym == SDLK_UP) {
                        *guideStep = 3;
                    }
                    break;
                case 3: // Flèche bas
                    if (event->key.keysym.sym == SDLK_DOWN) {
                        *guideStep = 4;
                    }
                    break;
                case 4: // Z pour Joueur 2
                    if (event->key.keysym.sym == SDLK_z) {
                        *guideStep = 5;
                    }
                    break;
                case 5: // Q pour Joueur 2
                    if (event->key.keysym.sym == SDLK_q) {
                        *guideStep = 6;
                    }
                    break;
                case 6: // S pour Joueur 2
                    if (event->key.keysym.sym == SDLK_s) {
                        *guideStep = 7;
                    }
                    break;
                case 7: // D pour Joueur 2
                    if (event->key.keysym.sym == SDLK_d) {
                        *guideStep = 8;
                    }
                    break;
                case 8: // ENTER pour commencer le jeu
                    if (event->key.keysym.sym == SDLK_RETURN) {
                        *guideVisible = false; // Masquer le guide quand on appuie sur ENTER
                    }
                    break;
            }
        }

        // Affichage du texte pour l'étape en cours
        SDL_Surface* texte = TTF_RenderText_Solid(font, messages[*guideStep], white);
        SDL_Surface* shadowText = TTF_RenderText_Solid(font, messages[*guideStep], shadow);

        // Centrer le texte
        SDL_Rect centeredPos = pos;
        centeredPos.x -= texte->w / 2;

        // Affichage de l'ombre du texte (décalée)
        SDL_Rect shadowPos = centeredPos;
        shadowPos.x += 2;
        shadowPos.y += 2;

        // Afficher l'ombre et le texte principal
        SDL_BlitSurface(shadowText, NULL, screen, &shadowPos);
        SDL_BlitSurface(texte, NULL, screen, &centeredPos);

        // Libérer les surfaces
        SDL_FreeSurface(texte);
        SDL_FreeSurface(shadowText);
    }

    TTF_CloseFont(font);
}

