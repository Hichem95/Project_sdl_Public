
#ifndef COLLISION_BB_H
#define COLLISION_BB_H

#include <SDL/SDL.h>

int CollisionBB(SDL_Rect joueur, SDL_Rect plateforme);
int CollisionBBAvecEffet(SDL_Rect joueur, SDL_Rect plateforme, SDL_Surface *screen);

#endif
