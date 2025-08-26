#include "minimap.h"
#include "header.h"
#include <SDL/SDL_image.h>
#include <stdio.h>


void afficherEnemyMinimap(Minimap *m, SDL_Surface *screen, SDL_Rect ennemiPos) {
    SDL_Rect posEnnemiMinimap;
    posEnnemiMinimap.x = m->positionImage.x + (ennemiPos.x * m->largeurMinimap / m->largeurFond);
    posEnnemiMinimap.y = m->positionImage.y + (ennemiPos.y * m->hauteurMinimap / m->hauteurFond);
    posEnnemiMinimap.w = 7;
    posEnnemiMinimap.h = 7;

    SDL_Surface* pointVert = SDL_CreateRGBSurface(0, 7, 7, 32, 0, 0, 0, 0);
    SDL_FillRect(pointVert, NULL, SDL_MapRGB(pointVert->format, 0, 255, 0)); // Vert pour l’ennemi
    SDL_BlitSurface(pointVert, NULL, screen, &posEnnemiMinimap);
    SDL_FreeSurface(pointVert);
}
void initialiserMinimap(Minimap *m, const char *fichierMinimap, const char *fichierPoint, int largeurFond, int level, int posX, int posY) {
    m->image = SDL_LoadBMP(fichierMinimap);
    m->point = SDL_LoadBMP(fichierPoint);
    m->positionImage.x = posX;
    m->positionImage.y = posY;
    m->largeurFond = largeurFond;
    m->hauteurFond = 800;
    if (m->image) {
        m->largeurMinimap = m->image->w;
        m->hauteurMinimap = m->image->h;
    } else {
        m->largeurMinimap = 160;
        m->hauteurMinimap = 32;
    }
}

void afficherMinimap(Minimap *m, SDL_Surface *screen, SDL_Rect joueurPos) {
    if (!m->image || !m->point || !screen) return;
    SDL_BlitSurface(m->image, NULL, screen, &m->positionImage);
    m->positionPoint.x = m->positionImage.x + (joueurPos.x * m->largeurMinimap) / m->largeurFond;
    m->positionPoint.y = m->positionImage.y + (joueurPos.y * m->hauteurMinimap) / m->hauteurFond;
    SDL_BlitSurface(m->point, NULL, screen, &m->positionPoint);
}
