#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    SDL_Surface *m;  
    SDL_Rect posm;     
    SDL_Surface *bh;  
    SDL_Rect posbh;     
} minimap;


void MAJMinimap(SDL_Rect posJoueur, minimap *m, SDL_Rect camera, int redimensionnement);
void AfficherMinimap(minimap m, SDL_Surface *screen);
void LibererMinimap(minimap *m);

#endif // MINIMAP_H
