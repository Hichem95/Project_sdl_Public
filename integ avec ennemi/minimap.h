#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL/SDL.h>

typedef struct {
    SDL_Surface *image;
    SDL_Surface *point;
    SDL_Rect positionImage;
    SDL_Rect positionPoint;
    int largeurFond;
    int hauteurFond;
    int largeurMinimap;
    int hauteurMinimap;
} Minimap;

void initialiserMinimap(Minimap *m, const char *fichierMinimap, const char *fichierPoint, int largeurFond, int level, int posX, int posY);
void afficherEnemyMinimap(Minimap *m, SDL_Surface *screen, SDL_Rect ennemiPos);
void afficherMinimap(Minimap *m, SDL_Surface *screen, SDL_Rect joueurPos);

#endif
