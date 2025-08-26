#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


typedef struct {
    SDL_Surface *image;
    SDL_Rect pos;
    SDL_Rect original_pos;
    SDL_Rect target_rect;
    SDL_Rect perfect_place;
    int is_dragging;
    int offset_x, offset_y;
    int is_correct;
    char* name;
} Piece;

 

 

SDL_Surface* render_text(const char* text, TTF_Font* font, SDL_Color color);
 

void randomize_positions(Piece pieces[], int screen_width, int screen_height);

 
SDL_Surface *load_image(const char* path);

#endif

