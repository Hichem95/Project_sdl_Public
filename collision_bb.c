
#include <SDL/SDL.h>
#include "collision_bb.h"
#include "animation_background.h"
#include "header.h"

int CollisionBB(SDL_Rect joueur, SDL_Rect plateforme) {
    if ((joueur.x + joueur.w < plateforme.x) || (joueur.x > plateforme.x + plateforme.w) ||
        (joueur.y + joueur.h < plateforme.y) || (joueur.y > plateforme.y + plateforme.h)) {
        return 0;
    } else {
        return 1;
    }
}

int CollisionBBAvecEffet(SDL_Rect joueur, SDL_Rect plateforme, SDL_Surface *screen) {
    if ((joueur.x + joueur.w < plateforme.x) || (joueur.x > plateforme.x + plateforme.w) ||
        (joueur.y + joueur.h < plateforme.y) || (joueur.y > plateforme.y + plateforme.h)) {
        return 0;
    } else {
        flashOnPlatform(screen, plateforme);
        return 1;
    }
}
