#include "header.h"
#include "animation_background.h"
#include <SDL/SDL_mixer.h>
#include <unistd.h>

void flashOnPlatform(SDL_Surface *screen, SDL_Rect platformRect) {
    SDL_Surface *flash = SDL_CreateRGBSurface(SDL_HWSURFACE, platformRect.w, platformRect.h, 32, 0, 0, 0, 0);
    SDL_FillRect(flash, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_BlitSurface(flash, NULL, screen, &platformRect);
    SDL_Flip(screen);
    usleep(100000);
    SDL_FreeSurface(flash);
}

void playBipSound() {
    Mix_Chunk *bip = Mix_LoadWAV("sound/bip.wav");
    if (bip != NULL) {
        Mix_PlayChannel(-1, bip, 0);
    }
}
