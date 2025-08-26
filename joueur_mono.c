#include "meilleur.h"
#include "minimap.h"
#include "header.h"
#include "background.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#define MAX_PROJECTILES 40
#define SCREEN_WIDTH 1204
#define SCREEN_HEIGHT 800


//variables globales
SDL_Surface *screen;
SDL_Event event;
joueur j;
minibox box;
AnimatedSprite *enemy;
projectile projectiles[MAX_PROJECTILES];
bool afficherGuide = true;


int joueur_mono(int selectav1, int selectav2, int selectin1, int selectin2) {
    //variables
    int quitter = 0;
    float vx = 1, vy = 0;
    int jump = 0;
    int attack = 0;
    int right=0, left=0, up=0, a=0;
    int i;
    int collision1;
    int hit_score = 0;
    Uint32 last_hit_time1 = 0;
    srand(time(NULL));
    Uint32 startTime = SDL_GetTicks();
    bool levelComplete = false;
    bool gameOver = false;

    int background_width = 2667;
    int background_height = 900;
    //initialisation de sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        SDL_Quit(); return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("ARCADECLASSIC.TTF", 24);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

     

    screen = SDL_SetVideoMode(1204, 800, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Erreur création écran : %s\n", SDL_GetError());
        IMG_Quit(); SDL_Quit(); return 1;
    }
 

    // Variables pour la caméra
    SDL_Rect camera = {0, 0, background_width,  background_height};  // La caméra est initialement à 0, 0 et couvre tout l'écran

    //debut du prog (utilisé pour score j1)
    Uint32 start_time = SDL_GetTicks();
     
    //creation des hitbox et initialisation des joueurs 
    j.image = create_color_surface(200, 220, 255, 0, 0);
    init_joueur(&j, 0, 430, 700, 100, 100);

	//création d'ennemi:
	init_box(&box);
	// load the two animations
	if (!init_sprite(&box.spriteRight, "sprite/spriteRight.png")) {
    		printf("Failed to load right-facing enemy\n");
    	exit(1);
	}
	if (!init_sprite(&box.spriteLeft,  "sprite/spriteLeft.png")) {
    		printf("Failed to load left -facing enemy\n");
    	exit(1);
	}
	 
	init_projectiles(projectiles, MAX_PROJECTILES);
	
    if (selectav1){
    //chargement des sprites du 1er sprite et init de facing 
    chargement_images(j.sprites, "sprite/sprite_idle_right.png", 0);
    chargement_images(j.sprites, "sprite/sprite_idle_left.png", 1);
    chargement_images(j.sprites, "sprite/sprite_run_right.png", 2);
    chargement_images(j.sprites, "sprite/sprite_run_left.png", 3);
    chargement_images(j.sprites, "sprite/sprite_jump_right.png", 4);
    chargement_images(j.sprites, "sprite/sprite_jump_left.png", 5);
    chargement_images(j.sprites, "sprite/sprite_attack_right.png", 6);
    chargement_images(j.sprites, "sprite/sprite_attack_left.png", 7);
    j.facing = FACING_RIGHT;
    }
   else if (selectav2){
    //chargement des sprites du 2eme sprite et init de facing 
    chargement_images(j.sprites, "sprite/sprite2_idle_right.png", 0);
    chargement_images(j.sprites, "sprite/sprite2_idle_left.png", 1);
    chargement_images(j.sprites, "sprite/sprite2_run_right.png", 2);
    chargement_images(j.sprites, "sprite/sprite2_run_left.png", 3);
    chargement_images(j.sprites, "sprite/sprite2_jump_right.png", 4);
    chargement_images(j.sprites, "sprite/sprite2_jump_left.png", 5);
    chargement_images(j.sprites, "sprite/sprite2_attack_right.png", 6);
    chargement_images(j.sprites, "sprite/sprite2_attack_left.png", 7);
    j.facing = FACING_RIGHT;}

    //init bg
    int guideStep = 0;
    Background bg;
    initialiserBackground(&bg, "img1/bg1.png", "img1/bg1.png");
    //minimap init
    Minimap m;
    int level = 0;
    initialiserMinimap(&m, "img1/minimap1.bmp", "img1/pointRouge.bmp", background_width, level, 10, 10);
	

 

        
    while (!quitter) {
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quitter = 1;


            	 else if (event.key.keysym.sym == SDLK_F1)
                afficherGuide = true, guideStep = 0;
		
        
	 else if (event.key.keysym.sym == SDLK_k)
                meilleur(j);
            else if (event.type == SDL_KEYDOWN) {
                
                     
                   // controls du j1
                    if (selectin2==1){
                    switch (event.key.keysym.sym) {
                    case SDLK_RIGHT: right = 1; break;
                    case SDLK_LEFT: left = 1; break;
                    case SDLK_UP: up = 1; break;
                    case SDLK_KP0: attack = 1; break;
                    case SDLK_m: a = 1; break;}
                   }
                    else if (selectin1==1){
                    switch (event.key.keysym.sym) {
                    case SDLK_d: right = 1; break;
                    case SDLK_q: left = 1; break;
                    case SDLK_z: up = 1; break;
                    case SDLK_f: attack = 1; break;
                    case SDLK_a: a = 1; break;}
                }


            } else if (event.type == SDL_KEYUP) {
                
                    //joueur 1
                    if (selectin2==1){
                    switch (event.key.keysym.sym) {
                    case SDLK_RIGHT: right = 0; break;
                    case SDLK_LEFT: left = 0; break;
                    case SDLK_UP: up = 0; break;
                    case SDLK_m: a = 0; break;
                    case SDLK_KP0: attack = 0; break;}
                   }
                    else if (selectin1==1){
                    switch (event.key.keysym.sym) {
                    case SDLK_d: right = 0; break;
                    case SDLK_q: left = 0; break;
                    case SDLK_z: up = 0; break;
                    case SDLK_f: attack = 0; break;
                    case SDLK_a: a = 0; break;}
                }
            }

}

 
        //mouvement des joueurs 
        deplacer_joueur(&j, screen, right, left, up, attack, a, &jump, &vx, &vy);
	  // limites
	if (j.pos.x < 0) j.pos.x = 0;
	if (j.pos.x > background_width - j.pos.w) j.pos.x = background_width - j.pos.w;
	if (j.pos.y < 0) j.pos.y = 0;
	if (j.pos.y > background_height - j.pos.h) j.pos.y = background_height - j.pos.h;

         //scroll
        bg.cameraSolo.x = j.pos.x + j.pos.w + 300;
        bg.cameraSolo.y = j.pos.y + j.pos.h -300;
	updateBackground(&bg);
	displaySingleBackground(screen, &bg);
        
       
	 
	//ennemi
	if (box.sante && rand() % 100 == 0) { // ~1% chance every frame
    		throw_projectile(projectiles, MAX_PROJECTILES, box.rect);
	}
	 
	update(&box, &j,attack);
	update_projectiles(projectiles, MAX_PROJECTILES, &j);
	draw(screen, &box);
	draw_health_bar(screen, &box);
	draw_projectiles(screen, projectiles, MAX_PROJECTILES);
    	if (box.facingLeft) {
        	enemy = &box.spriteLeft;
    	} else {
        	enemy = &box.spriteRight;
    	}
	update_sprite(enemy);
	
	int sprite_x = box.rect.x + (box.rect.w - FRAME_WIDTH)/2;
	

	//  the sprite position(ennemi):
	if (sprite_x + FRAME_WIDTH < 0) {
    	// fully off left → wrap to right
    		sprite_x = SCREEN_WIDTH;
	}
	else if (sprite_x > SCREEN_WIDTH) {
    	// fully off right → wrap to left
   		 sprite_x = -FRAME_WIDTH;
	}


	
	int sprite_y = box.rect.y + (box.rect.h - FRAME_HEIGHT);
	draw_sprite(screen, enemy, &box, sprite_x, sprite_y);


   
        if (box.sante > 0) {
            //handleing life, scores, collision if ennemy is alive sinon ils s'arretent 
            collision1 = check_collision(j.pos, box.rect);
            gestion_vie(&j, collision1, &last_hit_time1, attack);
            if (collision1) rebond_colli(&j, &box, &jump, &vy);
            handle_attack(&j, &box, &attack, &last_hit_time1, &hit_score);
            gestion_score(&j, start_time, font, hit_score); 
           }
   


 
        //rendering 
        
        
        for (i = 0; i < j.life_count; i++) SDL_BlitSurface(j.vie[i], NULL, screen, &j.posV[i]);
        
        if (j.life_count > 0) {
            //SDL_BlitSurface(j.image, NULL, screen, &j.pos);
            SDL_BlitSurface(j.sprites[j.animation_state], &j.frame, screen, &j.pos);
            
            SDL_BlitSurface(j.score, NULL, screen, &j.posS);
        }
        Uint32 temps = afficherTempsJeu(startTime);
        char buffer[50];
        sprintf(buffer, "Temps de jeu : %u s", temps);
        SDL_Color white = {255, 255, 255};
        SDL_Surface* txt = TTF_RenderText_Solid(font, buffer, white);
        SDL_Rect pos = {900, 10, 0, 0};
        SDL_BlitSurface(txt, NULL, screen, &pos);
        SDL_FreeSurface(txt);
	
	if (afficherGuide){
            afficherGuideTouches(screen, &event, &afficherGuide, &guideStep);
    	}

	
	if (box.sante == 0 && level == 0 && !levelComplete) {
    		levelComplete = true;
	}
 
	if (j.life_count <= 0) {
    		gameOver = true;
	}
    //levelcomplete:
    if (levelComplete) {
        //  pause.c 
        int action = levelcomplete();
  
        if (action == 1) {
            level = 1;
            init_box(&box);
            box.sante++;
            box.rect.x = 1200; 
	    box.rect.y = 430;
            initialiserBackground(&bg, "img1/bg2.png","img1/bg2.png");
            initialiserMinimap(&m, "img1/minimap2.bmp","img1/pointRouge.bmp",background_width, level, 10, 10);
            levelComplete = false;
        } else {
            quitter = 1;  // menu
        }
        continue;     
    }

    if (gameOver) {
        //  pause.c 
        
        int action = gameover();  
        if (action == 1) {
            level = 0;
            init_box(&box);
            initialiserBackground(&bg, "img1/bg1.png","img1/bg1.png");
            initialiserMinimap(&m, "img1/minimap1.bmp","img1/pointRouge.bmp",background_width, level, 10, 10);
            gameOver = false;
        } else {
            quitter = 1;
        }
        continue;
    }

 	          //afficher minimap
        afficherMinimap(&m, screen, j.pos); 
        afficherEnemyMinimap(&m, screen, box.rect); // ennemi 
        SDL_SetClipRect(screen, NULL);
        SDL_Flip(screen);
        SDL_Delay(16);
}//while:



   //freeing 
    for (i = 0; i < j.life_count; i++) if (j.vie[i])// SDL_FreeSurface(j.vie[i]); 
    if (j.image) SDL_FreeSurface(j.image);
    SDL_FreeSurface(j.score);
    SDL_FreeSurface(box.surface);
    for (int i = 0; i < MAX_PROJECTILES; i++)
        SDL_FreeSurface(projectiles[i].surface);

    free_sprite(enemy);
    

    IMG_Quit(); SDL_Quit();
    return 0;
}



