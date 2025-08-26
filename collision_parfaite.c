#include "collision_parfaite.h"
#include "animation_background.h"
#include "header.h"
#include <stdio.h>

Uint32 getpixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(Uint32 *)p;
        default: return 0;
    }
}

int CollisionParfaite(SDL_Surface *mask, SDL_Rect joueur) {
    if (!mask) return 0;

    for (int dx = 0; dx < joueur.w; dx++) {
        for (int dy = 0; dy < joueur.h; dy++) {
            int x = joueur.x + dx;
            int y = joueur.y + dy;
            if (x >= 0 && x < mask->w && y >= 0 && y < mask->h) {
                Uint32 pixel = getpixel(mask, x, y);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, mask->format, &r, &g, &b);
                if (r == 0 && g == 0 && b == 0) { playBipSound(); return 1; }
// collision avec le noir
            }
        }
    }
    return 0;
}
