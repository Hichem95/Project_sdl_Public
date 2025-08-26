#include "header.h"
#include <time.h>

#define SCREEN_WIDTH 1204
#define SCREEN_HEIGHT 800
#define CHASE_RANGE    300   // px
#define PATROL_SPEED    2
#define CHASE_SPEED     4
#define JUMP_VELOCITY  -12



//colors

SDL_Surface *create_color_surface(int w, int h , Uint8 r, Uint8 g, Uint8 b ) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    return surface;

	}



//initialisation enemy
 




void init_box(minibox *box){

        box->rect.x = 1200 ;
        box->rect.y = 430;
        box->rect.w = 120;
        box->rect.h = 220;
        box->xvitesse = -2;
        box->yvitesse = 0 ;

	box->accident = 0;
	box->sante= 3;
	box->state= PATROL;
	box->patrol_dir = -1;   // commence vers la gauche
	box->facingLeft=1;

	 

	

	box->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, box->rect.w, box->rect.h, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    	SDL_FillRect(box->surface, NULL, SDL_MapRGBA(box->surface->format, 0, 0, 0, 0));   
}


 


 

//fonction collision
int check_collision(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w <= b.x) 
	return 0;
    if (a.x >= b.x + b.w) 
	return 0;
    if (a.y + a.h <= b.y) 
	return 0;
    if (a.y >= b.y + b.h) 
	return 0;
    return 1;
}

 
//update enemy
void update(minibox *box, joueur *j,int attack1) {
    if (!box->sante) return;

  printf("box x: %d | player x: %d | chase: %d | facingLeft: %d\n", box->rect.x, j->pos.x, box->chase, box->facingLeft);


     
   // rest ennemi
    if(box->chase==0){
    if (box->rect.x <= 1000) {
	box->facingLeft=0;
    }else if(box->rect.x >= 1195){
	 box->facingLeft=1;
    }
    if(box->facingLeft){ box->rect.x += box->xvitesse;}
    else {box->rect.x -= box->xvitesse;}
    if(j->pos.x>=700){
	box->chase=1;
    }
    }

    if (box->chase) {
    // Move enemy toward player on X-axis
    if (box->rect.x < j->pos.x && j->pos.y >= 430) {
        box->facingLeft=0; 
	box->rect.x -= box->xvitesse;  // Move right
    } else if (box->rect.x > j->pos.x && j->pos.y >= 430) {
        box->facingLeft=1; 
	box->rect.x += box->xvitesse; // Move left
    }
    }


    // collision
    if (check_collision(box->rect, j->pos)&& attack1==1) {
	box->rect.x = 600;	
	  
        box->accident++; 
    }

    if (box->accident > 0) {
    box->sante--;
    box->accident = 0;

    if (box->sante <= 0) {
        box->sante = 0;
    }
}

}




//draw box
void draw(SDL_Surface* screen, minibox *box) {

    if (box->sante!=0) {
        SDL_BlitSurface(box->surface, NULL, screen, &box->rect);
    }
}




// PROJECTILES:

//init projectile:
void init_projectiles(projectile *p, int count) {
    for (int i = 0; i < count; i++) {
        p[i].surface = create_color_surface(10, 10, 255, 0, 0); // red bullet
        p[i].rect.x = 0;
        p[i].rect.y = 0;
        p[i].rect.w = 10;
        p[i].rect.h = 10;
        p[i].active = 0;
        p[i].speed = 8;
    }
}

//attack:
void throw_projectile(projectile *p, int count, SDL_Rect start_pos) {
    for (int i = 0; i < count; i++) {
        if (!p[i].active) {
            p[i].active = 1;

            // Start from the LEFT of the box
            p[i].rect.x = start_pos.x - 10; // 10 is projectile width
            p[i].rect.y = start_pos.y + (start_pos.h / 2) - 5; 

            p[i].rect.w = 10;
            p[i].rect.h = 10;

            p[i].vx = -10; // Move to the left

            p[i].surface = create_color_surface(10, 10, 255, 255, 255); // White projectile

            break;
        }
    }
}

//update projectile:
void update_projectiles(projectile *p, int count, joueur *j) {
    for (int i = 0; i < count; i++) {
        if (!p[i].active) continue;
        p[i].rect.x += p[i].vx;

        if (p[i].rect.x < 0) {
            p[i].active = 0;
            continue;
        }

        // Check for collision with obstacle
       if (check_collision(p[i].rect, j->pos)) {
            p[i].active = 0;  // Deactivate projectile
            j->life_count--;    // Reduce obstacle health
               
	}
        
    }
}

//draw projectile:
void draw_projectiles(SDL_Surface* screen, projectile *p, int count) {
    for (int i = 0; i < count; i++) {
        if (p[i].active)
            SDL_BlitSurface(p[i].surface, NULL, screen, &p[i].rect);
    }
}


//spritesheets:

//initialisation:
int init_sprite(AnimatedSprite *sprite, const char* filepath) {
	 
    sprite->spritesheet = IMG_Load(filepath);
    if (!sprite->spritesheet) return 0;

    SDL_SetColorKey(sprite->spritesheet, SDL_SRCCOLORKEY, SDL_MapRGB(sprite->spritesheet->format, 255, 0, 255));

    for (int i = 0; i < TOTAL_FRAMES; i++) {
        sprite->clips[i].x = i * FRAME_WIDTH;
        sprite->clips[i].y = 0;
        sprite->clips[i].w = FRAME_WIDTH;
        sprite->clips[i].h = FRAME_HEIGHT;
    }

    sprite->current_frame = 0;
    sprite->last_frame_time = SDL_GetTicks();
    sprite->frame_delay = 120;
 

    return 1;
}

//update:
void update_sprite(AnimatedSprite* sprite) {
    Uint32 now = SDL_GetTicks();

    if (now > sprite->last_frame_time + sprite->frame_delay) {
        sprite->current_frame = (sprite->current_frame + 1) % TOTAL_FRAMES;
        sprite->last_frame_time = now;
    }
    
	


}

//draw:
void draw_sprite(SDL_Surface* screen, AnimatedSprite* sprite, minibox *box, int x, int y) {
    if (box->sante != 0) {  // Only draw if box is alive
        SDL_Rect dest = {x, y, FRAME_WIDTH, FRAME_HEIGHT};
        SDL_BlitSurface(sprite->spritesheet, &sprite->clips[sprite->current_frame], screen, &dest);
    }
}

//free:
void free_sprite(AnimatedSprite* sprite) {
    SDL_FreeSurface(sprite->spritesheet);
}

//Health bar
void draw_health_bar(SDL_Surface* screen, minibox *box) {
    int bar_width = 20;
    int bar_height = 10;
    int spacing = 5;
    int start_x = box->rect.x + (box->rect.w - (3 * bar_width + 2 * spacing)) / 2;
    int y = box->rect.y - 15;

    for (int i = 0; i < box->sante; i++) {
        SDL_Rect bar_rect = { start_x + i * (bar_width + spacing), y, bar_width, bar_height };
        SDL_FillRect(screen, &bar_rect, SDL_MapRGB(screen->format, 255, 0, 0)); // red bars
    }
}

 












