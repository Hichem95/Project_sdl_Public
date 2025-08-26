#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include "enigme.h"
#include "header.h"


#define NUM_PIECES 3
#define TIMER_DURATION 15
#define TOLERANCE 30


SDL_Surface* render_text(const char* text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, color);
    if (!text_surface) {
        fprintf(stderr, "Erreur rendu texte: %s\n", TTF_GetError());
        return NULL;
    }
    return text_surface;
}

void randomize_positions(Piece pieces[], int screen_width, int screen_height) {
    srand(time(NULL));
    for (int i = 0; i < NUM_PIECES; i++) {
        pieces[i].original_pos.x = 50 + rand() % (screen_width - pieces[i].image->w - 100);
        pieces[i].original_pos.y = 100 + rand() % (screen_height - pieces[i].image->h - 200);
        pieces[i].pos = pieces[i].original_pos;
    }
}
 

int enigme() {
    SDL_Surface *screen = NULL, *background = NULL;
    SDL_Event event;
    int quit = 0;
    Piece pieces[NUM_PIECES];
    int i;
    Uint32 start_time = 0;
    TTF_Font* font = NULL;
    SDL_Color text_color = {0, 0, 0, 255}; // Timer en noir

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "Erreur SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur SDL_ttf: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    screen = SDL_SetVideoMode(1195, 740, 32, SDL_SWSURFACE);
    if (!screen) {
        fprintf(stderr, "Erreur création fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_WM_SetCaption("Puzzle Game SDL 1.2", NULL);

    font = TTF_OpenFont("alagard.ttf", 36);
    if (!font) {
        fprintf(stderr, "Erreur chargement police: %s\n", TTF_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    background = load_image("pic/bg.png");
    if (!background) {
        TTF_CloseFont(font);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Configuration des pièces
    // Pièce 1
    pieces[0].name = "p1";
    pieces[0].image = load_image("pic/p1.png");
    pieces[0].original_pos.w = pieces[0].image->w;
    pieces[0].original_pos.h = pieces[0].image->h;

    pieces[0].perfect_place.x = 232;
    pieces[0].perfect_place.y = 162;
    pieces[0].perfect_place.w = pieces[0].image->w;
    pieces[0].perfect_place.h = pieces[0].image->h;

    pieces[0].target_rect.x = pieces[0].perfect_place.x - TOLERANCE;
    pieces[0].target_rect.y = pieces[0].perfect_place.y - TOLERANCE;
    pieces[0].target_rect.w = pieces[0].perfect_place.w + 2*TOLERANCE;
    pieces[0].target_rect.h = pieces[0].perfect_place.h + 2*TOLERANCE;

    // Pièce 2
    pieces[1].name = "p2";
    pieces[1].image = load_image("pic/p2.png");
    pieces[1].original_pos.w = pieces[1].image->w;
    pieces[1].original_pos.h = pieces[1].image->h;

    pieces[1].perfect_place.x = 598;
    pieces[1].perfect_place.y = 163;
    pieces[1].perfect_place.w = pieces[1].image->w;
    pieces[1].perfect_place.h = pieces[1].image->h;

    pieces[1].target_rect.x = pieces[1].perfect_place.x - TOLERANCE;
    pieces[1].target_rect.y = pieces[1].perfect_place.y - TOLERANCE;
    pieces[1].target_rect.w = pieces[1].perfect_place.w + 2*TOLERANCE;
    pieces[1].target_rect.h = pieces[1].perfect_place.h + 2*TOLERANCE;

    // Pièce 3
    pieces[2].name = "p3";
    pieces[2].image = load_image("pic/p3.png");
    pieces[2].original_pos.w = pieces[2].image->w;
    pieces[2].original_pos.h = pieces[2].image->h;

    pieces[2].perfect_place.x = 232;
    pieces[2].perfect_place.y = 162;
    pieces[2].perfect_place.w = pieces[2].image->w;
    pieces[2].perfect_place.h = pieces[2].image->h;

    pieces[2].target_rect.x = pieces[2].perfect_place.x - TOLERANCE;
    pieces[2].target_rect.y = pieces[2].perfect_place.y - TOLERANCE;
    pieces[2].target_rect.w = pieces[2].perfect_place.w + 2*TOLERANCE;
    pieces[2].target_rect.h = pieces[2].perfect_place.h + 2*TOLERANCE;

    for (i = 0; i < NUM_PIECES; i++) {
        if (!pieces[i].image) {
            for (int j = 0; j < i; j++) SDL_FreeSurface(pieces[j].image);
            SDL_FreeSurface(background);
            TTF_CloseFont(font);
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 1;
        }

        pieces[i].is_dragging = 0;
        pieces[i].is_correct = 0;
    }

    // Position aléatoire des pièces
    randomize_positions(pieces, screen->w, screen->h);

    start_time = SDL_GetTicks() / 1000;
    int time_left = TIMER_DURATION;
    int game_over = 0;
    int victory = 0;

    while (!quit) {
        Uint32 current_time = SDL_GetTicks() / 1000;
        time_left = TIMER_DURATION - (current_time - start_time);

        if (time_left <= 0 && !game_over) {
            game_over = 1;
            time_left = 0;
        }

        while (SDL_PollEvent(&event)) {
            if (game_over || victory) {
                if (event.type == SDL_QUIT ||
                    (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
                    (event.type == SDL_MOUSEBUTTONDOWN)) {
                    quit = 1;
                }
                continue;
            }

            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        for (i = 0; i < NUM_PIECES; i++) {
                            if (!pieces[i].is_correct &&
                                event.button.x >= pieces[i].pos.x &&
                                event.button.x < pieces[i].pos.x + pieces[i].pos.w &&
                                event.button.y >= pieces[i].pos.y &&
                                event.button.y < pieces[i].pos.y + pieces[i].pos.h) {

                                pieces[i].is_dragging = 1;
                                pieces[i].offset_x = event.button.x - pieces[i].pos.x;
                                pieces[i].offset_y = event.button.y - pieces[i].pos.y;
                                break;
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        for (i = 0; i < NUM_PIECES; i++) {
                            if (pieces[i].is_dragging) {
                                pieces[i].is_dragging = 0;

                                int piece_center_x = pieces[i].pos.x + pieces[i].pos.w / 2;
                                int piece_center_y = pieces[i].pos.y + pieces[i].pos.h / 2;

                                if (piece_center_x >= pieces[i].target_rect.x &&
                                    piece_center_x <= pieces[i].target_rect.x + pieces[i].target_rect.w &&
                                    piece_center_y >= pieces[i].target_rect.y &&
                                    piece_center_y <= pieces[i].target_rect.y + pieces[i].target_rect.h) {

                                    pieces[i].is_correct = 1;
                                    pieces[i].pos = pieces[i].perfect_place;
                                } else {
                                    pieces[i].pos = pieces[i].original_pos;
                                }
                            }
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    for (i = 0; i < NUM_PIECES; i++) {
                        if (pieces[i].is_dragging) {
                            pieces[i].pos.x = event.motion.x - pieces[i].offset_x;
                            pieces[i].pos.y = event.motion.y - pieces[i].offset_y;
                        }
                    }
                    break;
            }
        }

        SDL_BlitSurface(background, NULL, screen, NULL);


        for (i = 0; i < NUM_PIECES; i++) {
            SDL_BlitSurface(pieces[i].image, NULL, screen, &pieces[i].pos);
        }

        

        
        int all_correct = 1;
        for (i = 0; i < NUM_PIECES; i++) {
            if (!pieces[i].is_correct) {
                all_correct = 0;
                break;
            }
        }

        if (all_correct && !victory && !game_over) {
            victory = 1;
            SDL_Surface *message = load_image("pic/victory.png");
            if (message) {
                SDL_Rect msg_pos = {290, 250, 0, 0};
                SDL_BlitSurface(message, NULL, screen, &msg_pos);
                SDL_FreeSurface(message);
            }
            SDL_Flip(screen);
            SDL_Delay(3000);
            quit = 1;
        }

        if (game_over && !victory) {
            SDL_Surface *fail_msg = load_image("pic/echec.png"); 
            if (fail_msg) {
                SDL_Rect fail_rect = {290, 180, 0, 0}; 
                SDL_BlitSurface(fail_msg, NULL, screen, &fail_rect);
                SDL_FreeSurface(fail_msg);
            }
            SDL_Flip(screen);
            SDL_Delay(3000);
            quit = 1;
        }
	if(victory){
		main();
	
	}
        SDL_Flip(screen);
    }

    for (i = 0; i < NUM_PIECES; i++) {
        SDL_FreeSurface(pieces[i].image);
    }
    SDL_FreeSurface(background);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
 
