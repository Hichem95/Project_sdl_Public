
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "animation_background.h"
#include <stdio.h>
#include "minimap.h"
#include "collision_parfaite.h"
#include "collision_bb.h"
#include "header.h"
#include <SDL/SDL_mixer.h>


int main() {
      SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    SDL_EnableKeyRepeat(10, 10);
    SDL_Surface *screen = SDL_SetVideoMode(1440, 482, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) return 1;

    SDL_Surface *background1 = SDL_LoadBMP("img/background1.bmp");
    SDL_Surface *background2 = SDL_LoadBMP("img/background2.bmp");
    SDL_Surface *masque1 = SDL_LoadBMP("img/masque1.bmp");
    SDL_Surface *masque2 = SDL_LoadBMP("img/masque2.bmp");
    SDL_Surface *joueurImg = SDL_LoadBMP("img/joueur.bmp");

    SDL_Rect posJoueur = {100, 241, 20, 20};
    SDL_Rect joueurAffiche = posJoueur;

    SDL_Rect plateformeMobile = {200, 291, 100, 20};
    SDL_Rect plateformeDestructible = {290, 365, 120, 25};
    int sens = 1;
    int plateformeDestructibleActive = 1;

    Minimap m;
    int level = 1;
    initialiserMinimap(&m, "img/minimap1.bmp", "img/pointRouge.bmp", 1440, level);

    int continuer = 1;
    SDL_Event event;
    SDL_Rect tentative;

    while (continuer) {
        plateformeMobile.x += sens * 1;
        if (plateformeMobile.x < 150 || plateformeMobile.x > 350) sens *= -1;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) continuer = 0;

            tentative = posJoueur;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT: tentative.x += 5; break;
                    case SDLK_LEFT: tentative.x -= 5; break;
                    case SDLK_UP: tentative.y -= 5; break;
                    case SDLK_DOWN: tentative.y += 5; break;
                    default : break;
                }
            }

            SDL_Surface *currentMask = (level == 1) ? masque1 : masque2;
            int collision_detectee = CollisionParfaite(currentMask, tentative) ||
                                     CollisionBB(tentative, plateformeMobile) ||
                                     (plateformeDestructibleActive && CollisionBBAvecEffet(tentative, plateformeDestructible, screen));

            if (!collision_detectee) {
                posJoueur = tentative;
            }

            if (plateformeDestructibleActive && CollisionBBAvecEffet(tentative, plateformeDestructible, screen)) {
                plateformeDestructibleActive = 0;
                plateformeDestructible.x = -1000;
            }
        }

        if (level == 1 && posJoueur.x > background1->w - 50) {
            level = 2;
            initialiserMinimap(&m, "img/minimap2.bmp", "img/pointRouge.bmp", background2->w, level);
        }

        SDL_BlitSurface(level == 1 ? background1 : background2, NULL, screen, NULL);
        SDL_FillRect(screen, &plateformeMobile, SDL_MapRGB(screen->format, 0, 255, 0));

        if (plateformeDestructibleActive)
            SDL_FillRect(screen, &plateformeDestructible, SDL_MapRGB(screen->format, 255, 0, 0));

        joueurAffiche.x = posJoueur.x;
        joueurAffiche.y = posJoueur.y;
        SDL_BlitSurface(joueurImg, NULL, screen, &joueurAffiche);

        afficherMinimap(&m, screen, posJoueur);

        SDL_Flip(screen);
    }

    SDL_FreeSurface(background1);
    SDL_FreeSurface(background2);
    SDL_FreeSurface(masque1);
    SDL_FreeSurface(masque2);
    SDL_FreeSurface(joueurImg);
    SDL_FreeSurface(m.image);
    SDL_FreeSurface(m.point);
    Mix_CloseAudio();
SDL_Quit();

    return 0;
}
