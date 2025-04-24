#include "suiv.h"

#define SCREEN_WIDTH 1204
#define SCREEN_HEIGHT 800


//colors

SDL_Surface *create_color_surface(int w, int h , Uint8 r, Uint8 g, Uint8 b ) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    return surface;

	}



//initialisation enemy
 




void init_box(minibox *box){

        box->rect.x = 200 ;
        box->rect.y = 550;
        box->rect.w = 50;
        box->rect.h = 128;
        box->xvitesse = -5;
        box->yvitesse = 0 ;

	box->accident = 0;
	box->sante= 1;
	

	box->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, box->rect.w, box->rect.h, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    	SDL_FillRect(box->surface, NULL, SDL_MapRGBA(box->surface->format, 0, 0, 0, 0));   
}


 



void init_obstacle(obstacle *obs, int x, int y, int w, int h, int health) {
    obs->rect.x = x;
    obs->rect.y = y;
    obs->rect.w = w;
    obs->rect.h = h;
    obs->health = 2;
    obs->visible = 1;
    obs->surface = create_color_surface(w, h, 0, 0, 255); // blue obstacle
}


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

 






void update(minibox *box, obstacle *obs) {

        if (!box->sante) return; //skip dep if sante=0

        box->rect.x += box->xvitesse;

        // bounce
   
        if (box->rect.x + box->rect.w < 0){
		 box->rect.x = SCREEN_WIDTH;

	}else if (box->rect.x > SCREEN_WIDTH){
            box->xvitesse *= -1;

	}

        // collision
        if (obs->visible && check_collision(box->rect, obs->rect)) {


            box->xvitesse *= -1;
            box->accident++;

	}
        if (box->accident>= 3) {
                box->sante= 0; // Dead
		
            }
    
}




void draw(SDL_Surface* screen, minibox *box) {

    if (box->sante!=0) {
        SDL_BlitSurface(box->surface, NULL, screen, &box->rect);
    }
}




// PROJECTILES
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


void update_projectiles(projectile *p, int count, obstacle *obs) {
    for (int i = 0; i < count; i++) {
        if (!p[i].active) continue;
        p[i].rect.x += p[i].vx;

        if (p[i].rect.x < 0) {
            p[i].active = 0;
            continue;
        }

        // Check for collision with obstacle
       if (obs->visible && check_collision(p[i].rect, obs->rect)) {
            p[i].active = 0;  // Deactivate projectile
            obs->health--;    // Reduce obstacle health
            
            update_obstacle_color(obs);  // Update color based on health
            
            if (obs->health <= 0) {
                obs->visible = 0;  // Make obstacle disappear
	    }
	}
        
    }
}


void draw_projectiles(SDL_Surface* screen, projectile *p, int count) {
    for (int i = 0; i < count; i++) {
        if (p[i].active)
            SDL_BlitSurface(p[i].surface, NULL, screen, &p[i].rect);
    }
}


void draw_obstacle(SDL_Surface *screen, obstacle *obs) {
    if (obs->visible && obs->health > 0) {
        SDL_BlitSurface(obs->surface, NULL, screen, &obs->rect);
    }
}


void update_obstacle_color(obstacle *obs) {
    if (obs->health == 2) {
        // Initial state - blue
        obs->surface = create_color_surface(obs->rect.w, obs->rect.h, 0, 0, 255);
    } 
    else if (obs->health == 1) {
        // After first hit - orange
        obs->surface = create_color_surface(obs->rect.w, obs->rect.h, 255, 165, 0);
    }
    // When health reaches 0, it will become invisible
}




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

void update_sprite(AnimatedSprite* sprite) {
    Uint32 now = SDL_GetTicks();

    if (now > sprite->last_frame_time + sprite->frame_delay) {
        sprite->current_frame = (sprite->current_frame + 1) % TOTAL_FRAMES;
        sprite->last_frame_time = now;
    }
    
	


}

void draw_sprite(SDL_Surface* screen, AnimatedSprite* sprite, minibox *box, int x, int y) {
    if (box->sante != 0) {  // Only draw if box is alive
        SDL_Rect dest = {x, y, FRAME_WIDTH, FRAME_HEIGHT};
        SDL_BlitSurface(sprite->spritesheet, &sprite->clips[sprite->current_frame], screen, &dest);
    }
}

void free_sprite(AnimatedSprite* sprite) {
    SDL_FreeSurface(sprite->spritesheet);
}



 











