// main.c avec mini-map séparée pour les 2 joueurs
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "header.h"
#include "joueur.h"
#include "meilleur.h"
#include "minimap.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Surface* screen = SDL_SetVideoMode(1200, 800, 32, SDL_SWSURFACE);
    TTF_Font* font = TTF_OpenFont("alagard.ttf", 30);

    joueur j = {0}, j3 = {0}, j2 = {0};
    j.image = create_color_surface(247, 247, 255, 0, 0);
    j3.image = create_color_surface(247, 247, 0, 0, 255);
    j2.image = create_color_surface(100, 50, 0, 255, 0);

    init_joueur(&j, 300, 300, 0, 0, 0);
    init_joueur(&j3, 300, 1200, 0, 0, 0);
    init_joueur(&j2, 500, 500, 0, 0, 0);

    const char* sprite_files[8] = {
        "sprite/sprite_idle_right.png", "sprite/sprite_idle_left.png",
        "sprite/sprite_run_right.png", "sprite/sprite_run_left.png",
        "sprite/sprite_jump_right.png", "sprite/sprite_jump_left.png",
        "sprite/sprite_attack_right.png", "sprite/sprite_attack_left.png"
    };
    for (int i = 0; i < 8; i++) {
        chargement_images(j.sprites, (char*)sprite_files[i], i);
        chargement_images(j3.sprites, (char*)sprite_files[i], i);
    }

    Background bg;
    initialiserBackground(&bg, "img1/bg1.png", "img1/bg1.png");

    Minimap m, m2;
    int level = 0;
    int background_width = 3200;
    int background_height = 800;
    initialiserMinimap(&m, "img1/minimap1.bmp", "img1/pointRouge.bmp", background_width, level, 10, 10);
    initialiserMinimap(&m2, "img1/minimap1.bmp", "img1/pointRouge.bmp", background_width, level, 10, 400);

    bool isSplitScreen = false, afficherGuide = true;
    int guideStep = 0;
    Uint32 startTime = SDL_GetTicks(), last_hit1 = 0;
    int right = 0, left = 0, up = 0, atk = 0, a = 0;
    int right2 = 0, left2 = 0, up2 = 0, atk2 = 0, a2 = 0;
    int jump = 0, jump2 = 0;
    float vx = 3, vy = 0, vx2 = 3, vy2 = 0;
    int hit_score = 0;
    SDL_Event event;

    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return 0;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                afficherGuide = false;
            else if (event.key.keysym.sym == SDLK_F1)
                afficherGuide = true, guideStep = 0;
            else if (event.key.keysym.sym == SDLK_F2)
                meilleur(j);

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT: right = 1; break;
                    case SDLK_LEFT: left = 1; break;
                    case SDLK_UP: up = 1; break;
                    case SDLK_m: a = 1; break;
                    case SDLK_k: atk = 1; break;
                    case SDLK_d: right2 = 1; break;
                    case SDLK_q: left2 = 1; break;
                    case SDLK_z: up2 = 1; break;
                    case SDLK_f: atk2 = 1; break;
                    case SDLK_p: isSplitScreen = !isSplitScreen; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT: right = 0; break;
                    case SDLK_LEFT: left = 0; break;
                    case SDLK_UP: up = 0; break;
                    case SDLK_m: a = 0; break;
                    case SDLK_d: right2 = 0; break;
                    case SDLK_q: left2 = 0; break;
                    case SDLK_z: up2 = 0; break;
                    case SDLK_f: atk2 = 0; break;
                    case SDLK_k: atk = 0; break;
                }
            }
        }

        deplacer_joueur(&j, screen, right, left, up, atk, a, &jump, &vx, &vy);
        deplacer_joueur(&j3, screen, right2, left2, up2, atk2, a2, &jump2, &vx2, &vy2);
        handle_attack(&j, &j3, &atk, &last_hit1, &hit_score);
        gestion_score(&j, startTime, font, hit_score);
	gestion_score(&j3,startTime,font,hit_score);
        // limites
        if (j.pos.x < 0) j.pos.x = 0;
        if (j.pos.x > background_width - j.pos.w) j.pos.x = background_width - j.pos.w;
        if (j.pos.y < 0) j.pos.y = 0;
        if (j.pos.y > background_height - j.pos.h) j.pos.y = background_height - j.pos.h;

        if (j3.pos.x < 0) j3.pos.x = 0;
        if (j3.pos.x > background_width - j3.pos.w) j3.pos.x = background_width - j3.pos.w;
        if (j3.pos.y < 0) j3.pos.y = 0;
        if (j3.pos.y > background_height - j3.pos.h) j3.pos.y = background_height - j3.pos.h;

        // Caméras
        if (isSplitScreen) {
            bg.camera1.x = j.pos.x + j.pos.w / 2 - bg.camera1.w / 2;
            bg.camera1.y = j.pos.y + j.pos.h / 2 - bg.camera1.h / 2;
            bg.camera2.x = j3.pos.x + j3.pos.w / 2 - bg.camera2.w / 2;
            bg.camera2.y = j3.pos.y + j3.pos.h / 2 - bg.camera2.h / 2;
        } else {
            bg.cameraSolo.x = j.pos.x + j.pos.w / 2 - bg.cameraSolo.w / 2;
            bg.cameraSolo.y = j.pos.y + j.pos.h / 2 - bg.cameraSolo.h / 2;
        }
	
        updateBackground(&bg);

        if (isSplitScreen) {
            splitScreen(screen, &bg);

            SDL_SetClipRect(screen, &(SDL_Rect){0, 0, screen->w, screen->h / 2});
            SDL_BlitSurface(j.sprites[j.animation_state], &j.frame, screen,
                &(SDL_Rect){j.pos.x - bg.camera1.x, j.pos.y - bg.camera1.y, 0, 0});

            SDL_SetClipRect(screen, &(SDL_Rect){0, screen->h / 2, screen->w, screen->h / 2});
            SDL_BlitSurface(j3.sprites[j3.animation_state], &j3.frame, screen,
                &(SDL_Rect){j3.pos.x - bg.camera2.x, j3.pos.y - bg.camera2.y + screen->h / 2, 0, 0});

            SDL_SetClipRect(screen, NULL);
            afficherMinimap(&m, screen, j.pos);   // mini-map joueur 1
            afficherMinimap(&m2, screen, j3.pos); // mini-map joueur 2
	    // vie et score du joueur2
            for(int i=0;i<j3.life_count;i++){
                SDL_Rect v2 = j3.posV[i]; v2.y+=400;
                SDL_BlitSurface(j3.vie[i],NULL,screen,&v2);
            }
            if(j3.score){
                SDL_Rect sp2={150,440,0,0};
                SDL_BlitSurface(j3.score,NULL,screen,&sp2);

	}



        } else {
            displaySingleBackground(screen, &bg);
            SDL_SetClipRect(screen, NULL);
            SDL_BlitSurface(j.sprites[j.animation_state], &j.frame, screen,
                &(SDL_Rect){j.pos.x - bg.cameraSolo.x, j.pos.y - bg.cameraSolo.y, 0, 0});
            afficherMinimap(&m, screen, j.pos);
	
        }

        // Affichage de la vie et du score
        for (int i = 0; i < j.life_count; i++)
            SDL_BlitSurface(j.vie[i], NULL, screen, &j.posV[i]);

        if (j.score) {
            SDL_Rect scorePos = {10, 40, 0, 0};
            SDL_BlitSurface(j.score, NULL, screen, &scorePos);
        }


        Uint32 temps = afficherTempsJeu(startTime);
        char buffer[50];
        sprintf(buffer, "Temps de jeu : %u s", temps);
        SDL_Color white = {255, 255, 255};
        SDL_Surface* txt = TTF_RenderText_Solid(font, buffer, white);
        SDL_Rect pos = {900, 10, 0, 0};
        SDL_BlitSurface(txt, NULL, screen, &pos);
        SDL_FreeSurface(txt);

        if (temps >= 10 && level == 0) {
            level = 1;
            initialiserBackground(&bg, "img1/bg2.png", "img1/bg2.png");
            initialiserMinimap(&m, "img1/minimap2.bmp", "img1/pointRouge.bmp", background_width, level, 10, 10);
            initialiserMinimap(&m2, "img1/minimap2.bmp", "img1/pointRouge.bmp", background_width, level, 10, 400);
        } else if (temps >= 20 && level == 1) {
            level = 2;
        }

        if (afficherGuide1)
            afficherGuideTouches(screen, &event, &afficherGuide1, &guideStep);

        SDL_Flip(screen);
        SDL_Delay(16);
    }

    return 0;
}

