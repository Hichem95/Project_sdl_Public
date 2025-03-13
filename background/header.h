#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>

SDL_Surface *screen = NULL;
typedef struct{
SDL_Surface *bg1 ;
SDL_Surface *bg2 ;
SDL_Surface *bg3 ;
SDL_Rect camera_pos;
int direction;
}bg;
SDL_Surface *loadImage(const char *filename);
void Init Background(bg *b);
void freeBackground(Background* bg);
void Scrolling(bg *b,int dx,int dy,int direction);
