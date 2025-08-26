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
joueur j,j3;
minibox box;
AnimatedSprite enemy;
projectile projectiles[MAX_PROJECTILES];
afficherGuide1 = true;


int joueur_multiple ( int  selectav1, int selectav2, int selectin1, int selectin2) {
    //variables
    int quitter = 0;
    float vx = 5, vy = 0;
    float vx2 = 5, vy2 = 0;
    int i;
    int jump = 0, jump2 = 0;
    int attack = 0, attack2 = 0;
    int right = 0, left = 0, up = 0, a = 0;
    int right2 = 0, left2 = 0, up2 = 0, a2 = 0;
    int startj2 = 0;
    int collision1, collision2;
    int hit_score = 0, hit_score2 = 0;
    Uint32 last_hit_time1 = 0, last_hit_time2 = 0;
    srand(time(NULL));
    Uint32 startTime = SDL_GetTicks();
    bool levelComplete=false;
    bool gameOver=false;


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


    //debut du prog (utilisé pour score j1)
    Uint32 start_time = SDL_GetTicks();
     
    //creation des hitbox et initialisation des joueurs 
    j.image = create_color_surface(200, 220, 255, 0, 0);
    init_joueur(&j, 0, 430, 700, 100, 100);

    j3.image = create_color_surface(200, 220, 0, 0, 255);
    init_joueur(&j3, 0, 430, 700, 200, 200);

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
         
       // Variables pour la caméra
        SDL_Rect camera = {0, 0, screen->w, screen->h};  // La caméra est initialement à 0, 0 et couvre tout l'écran
	 //ennemi projectiles
	init_projectiles(projectiles, MAX_PROJECTILES);
     
   ////////////////// choix vestimentaire : tu choisis le premier et automatiquement l'autre et l'autre couleur 
    if (selectav1){
    //chargement des sprites du 1er joueur et init de facing 
    chargement_images(j.sprites, "sprite/sprite_idle_right.png", 0);
    chargement_images(j.sprites, "sprite/sprite_idle_left.png", 1);
    chargement_images(j.sprites, "sprite/sprite_run_right.png", 2);
    chargement_images(j.sprites, "sprite/sprite_run_left.png", 3);
    chargement_images(j.sprites, "sprite/sprite_jump_right.png", 4);
    chargement_images(j.sprites, "sprite/sprite_jump_left.png", 5);
    chargement_images(j.sprites, "sprite/sprite_attack_right.png", 6);
    chargement_images(j.sprites, "sprite/sprite_attack_left.png", 7);
    j.facing = FACING_RIGHT;
    //chargement des sprites du 2eme joueur et init de facing 
    chargement_images(j3.sprites, "sprite/sprite2_idle_right.png", 0);
    chargement_images(j3.sprites, "sprite/sprite2_idle_left.png", 1);
    chargement_images(j3.sprites, "sprite/sprite2_run_right.png", 2);
    chargement_images(j3.sprites, "sprite/sprite2_run_left.png", 3);
    chargement_images(j3.sprites, "sprite/sprite2_jump_right.png", 4);
    chargement_images(j3.sprites, "sprite/sprite2_jump_left.png", 5);
    chargement_images(j3.sprites, "sprite/sprite2_attack_right.png", 6);
    chargement_images(j3.sprites, "sprite/sprite2_attack_left.png", 7);
    j3.facing = FACING_RIGHT;}

   else if (selectav2) {
  //chargement des sprites du 1er joueur et init de facing 
    chargement_images(j.sprites, "sprite/sprite2_idle_right.png", 0);
    chargement_images(j.sprites, "sprite/sprite2_idle_left.png", 1);
    chargement_images(j.sprites, "sprite/sprite2_run_right.png", 2);
    chargement_images(j.sprites, "sprite/sprite2_run_left.png", 3);
    chargement_images(j.sprites, "sprite/sprite2_jump_right.png", 4);
    chargement_images(j.sprites, "sprite/sprite2_jump_left.png", 5);
    chargement_images(j.sprites, "sprite/sprite2_attack_right.png", 6);
    chargement_images(j.sprites, "sprite/sprite2_attack_left.png", 7);
    j.facing = FACING_RIGHT;
    //chargement des sprites du 2eme joueur et init de facing 
    chargement_images(j3.sprites, "sprite/sprite_idle_right.png", 0);
    chargement_images(j3.sprites, "sprite/sprite_idle_left.png", 1);
    chargement_images(j3.sprites, "sprite/sprite_run_right.png", 2);
    chargement_images(j3.sprites, "sprite/sprite_run_left.png", 3);
    chargement_images(j3.sprites, "sprite/sprite_jump_right.png", 4);
    chargement_images(j3.sprites, "sprite/sprite_jump_left.png", 5);
    chargement_images(j3.sprites, "sprite/sprite_attack_right.png", 6);
    chargement_images(j3.sprites, "sprite/sprite_attack_left.png", 7);
    j3.facing = FACING_RIGHT;}
   
  
   //init bg
    int guideStep = 0;
    Background bg;
    initialiserBackground(&bg, "img1/bg1.png", "img1/bg1.png");
    //minimap init
    Minimap m, m2;
    int level = 0;
    int background_width = 2667;
    int background_height = 900;
    initialiserMinimap(&m, "img1/minimap1.bmp", "img1/pointRouge.bmp", background_width, level, 10, 10);
    initialiserMinimap(&m2, "img1/minimap1.bmp", "img1/pointRouge.bmp", background_width, level, 10, 400);

        


    while (!quitter) {
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
///////////////// choix du input si tu choisis les fleches dans le menu*
	if (selectin2){ 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quitter = 1;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                   // controls du j1
                    case SDLK_RIGHT: right = 1; break;
                    case SDLK_LEFT: left = 1; break;
                    case SDLK_UP: up = 1; break;
                    case SDLK_KP0: attack = 1; break;
                    case SDLK_m: a = 1; break;
                   // controls du j2
                    case SDLK_d: right2 = 1; break;
                    case SDLK_q: left2 = 1; break;
                    case SDLK_z: up2 = 1; break;
                    case SDLK_f: attack2 = 1; break;
                    case SDLK_a: a2 = 1; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    //joueur 1
                    case SDLK_RIGHT: right = 0; break;
                    case SDLK_LEFT: left = 0; break;
                    case SDLK_UP: up = 0; break;
                    case SDLK_m: a = 0; break;
                    case SDLK_KP0: attack = 0; break;
                   //jouer 2
                    case SDLK_d: right2 = 0; break;
                    case SDLK_q: left2 = 0; break;
                    case SDLK_z: up2 = 0; break;
                    case SDLK_f: attack2 = 0; break;
                    case SDLK_a: a2 = 0; break;
                }
            }
        }

}
///////////////// choix du input si tu choisis les lettres dans le menu 

 if (selectin1){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quitter = 1;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                   // controls du j1
                    case SDLK_RIGHT: right2 = 1; break;
                    case SDLK_LEFT: left2 = 1; break;
                    case SDLK_UP: up2 = 1; break;
                    case SDLK_KP0: attack2 = 1; break;
                    case SDLK_m: a2 = 1; break;
                   // controls du j2
                    case SDLK_d: right = 1; break;
                    case SDLK_q: left = 1; break;
                    case SDLK_z: up = 1; break;
                    case SDLK_f: attack = 1; break;
                    case SDLK_a: a = 1; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    //joueur 1
                    case SDLK_RIGHT: right2 = 0; break;
                    case SDLK_LEFT: left2 = 0; break;
                    case SDLK_UP: up2 = 0; break;
                    case SDLK_m: a2 = 0; break;
                    case SDLK_KP0: attack2 = 0; break;
                   //jouer 2
                    case SDLK_d: right = 0; break;
                    case SDLK_q: left = 0; break;
                    case SDLK_z: up = 0; break;
                    case SDLK_f: attack = 0; break;
                    case SDLK_a: a = 0; break;
                }
            }
        }
}

// limites
        if (j.pos.x < 0) j.pos.x = 0;
        if (j.pos.x > background_width - j.pos.w) j.pos.x = background_width - j.pos.w;
        if (j.pos.y < 0) j.pos.y = 0;
        if (j.pos.y > background_height - j.pos.h) j.pos.y = background_height - j.pos.h;

        if (j3.pos.x < 0) j3.pos.x = 0;
        if (j3.pos.x > background_width - j3.pos.w) j3.pos.x = background_width - j3.pos.w;
        if (j3.pos.y < 0) j3.pos.y = 0;
        if (j3.pos.y > background_height - j3.pos.h) j3.pos.y = background_height - j3.pos.h;
     /// camera
            bg.camera1.x = j.pos.x + j.pos.w / 2 - bg.camera1.w / 2;
            bg.camera1.y = j.pos.y + j.pos.h / 2 - bg.camera1.h / 2;
            bg.camera2.x = j3.pos.x + j3.pos.w / 2 - bg.camera2.w / 2;
            bg.camera2.y = j3.pos.y + j3.pos.h / 2 - bg.camera2.h / 2;
        //update bg    
        updateBackground(&bg);

       //affichage
            splitScreen(screen, &bg);

            SDL_SetClipRect(screen, &(SDL_Rect){0, 0, screen->w, screen->h / 2});
            SDL_BlitSurface(j.sprites[j.animation_state], &j.frame, screen,
                &(SDL_Rect){j.pos.x - bg.camera1.x, j.pos.y - bg.camera1.y, 0, 0});

            SDL_SetClipRect(screen, &(SDL_Rect){0, screen->h / 2, screen->w, screen->h / 2});
            SDL_BlitSurface(j3.sprites[j3.animation_state], &j3.frame, screen,
                &(SDL_Rect){j3.pos.x - bg.camera2.x, j3.pos.y - bg.camera2.y + screen->h / 2, 0, 0});

            SDL_SetClipRect(screen, NULL);



         
        //mouvement des joueurs 
        deplacer_joueur(&j, screen, right, left, up, attack, a, &jump, &vx, &vy);
        deplacer_joueur(&j3, screen, right2, left2, up2, attack2, a2, &jump2, &vx2, &vy2);

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
        	enemy = box.spriteLeft;
    	} else {
        	enemy = box.spriteRight;
    	}
	update_sprite(&enemy);
	
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
	draw_sprite(screen, &enemy, &box, sprite_x, sprite_y);

   
        if (box.sante> 0) {
            //handleing life, scores, collision if ennemy is alive sinon ils s'arretent 
            collision1 = check_collision(j.pos, box.rect);
            gestion_vie(&j, collision1, &last_hit_time1, attack);
            if (collision1) rebond_colli(&j, &box, &jump, &vy);
            handle_attack(&j, &box, &attack, &last_hit_time1, &hit_score);
            gestion_score(&j, start_time, font, hit_score);
                      
             //ennemi avec j3
                collision2 = check_collision(j3.pos, box.rect);
                gestion_vie(&j3, collision2, &last_hit_time2, attack2);
                if (collision2) rebond_colli(&j3, &box, &jump2, &vy2);
                gestion_score(&j3, start_time, font, hit_score2);
                handle_attack(&j3, &box, &attack2, &last_hit_time2, &hit_score2); 
            
           }
       


 
        //rendering 
        for (i = 0; i < j.life_count; i++) SDL_BlitSurface(j.vie[i], NULL, screen, &j.posV[i]);
        if (j.life_count > 0) {
            SDL_BlitSurface(j.score, NULL, screen, &j.posS);
        }

        //j3
            for (i = 0; i < j3.life_count; i++) SDL_BlitSurface(j3.vie[i], NULL, screen, &j3.posV[i]);
            if (j3.life_count > 0) {
            //SDL_BlitSurface(j3.image, NULL, screen, &j3.pos);  //hitbox
            SDL_BlitSurface(j3.score, NULL, screen, &j3.posS);}
        



        if (afficherGuide1)
            afficherGuideTouches(screen, &event, &afficherGuide1, &guideStep);


////////////////


if (box.sante == 0 && level == 0 && !levelComplete) {
    		levelComplete = true;
	}
 
	if (j.life_count <= 0 || j3.life_count<= 0 ) {
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
        afficherMinimap(&m, screen, j.pos);   // mini-map joueur 1
        afficherMinimap(&m2, screen, j3.pos); // mini-map joueur 2
        afficherEnemyMinimap(&m, screen, box.rect); // ennemi
        SDL_Flip(screen);
        SDL_Delay(16);
    }



   //freeing 
    for (i = 0; i < j.life_count; i++) if (j.vie[i])// SDL_FreeSurface(j.vie[i]); 
    if (j.image) SDL_FreeSurface(j.image);
    SDL_FreeSurface(box.surface);
    for (int i = 0; i < MAX_PROJECTILES; i++)
        SDL_FreeSurface(projectiles[i].surface);

    free_sprite(&enemy);
    SDL_FreeSurface(j.score);
    IMG_Quit(); SDL_Quit();
    return 0;
}



