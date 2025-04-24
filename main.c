#include "suiv.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <time.h>


#define MAX_PROJECTILES 40
#define SCREEN_WIDTH 1204
#define SCREEN_HEIGHT 800

 

int main() {
    
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
    SDL_Surface* screen = SDL_SetVideoMode(1204, 800, 32, SDL_SWSURFACE);

 

 
 
int running=1;
SDL_Event event;

srand(time(NULL));
minibox box;
AnimatedSprite enemy;

projectile projectiles[MAX_PROJECTILES];
    init_projectiles(projectiles, MAX_PROJECTILES);

 



init_box(&box);
	if (!init_sprite(&enemy, "sprite/enemy.png")) {
    printf("Failed to load sprite: %s\n", IMG_GetError());
    return 1;
}
 
obstacle obs;
init_obstacle(&obs, 0, 450, 50, 200, 3);

 


    while (running) {
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	draw(screen, &box);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
		running=0;
        }

	 
	
	 

// Auto-throw with random chance

if (box.sante && rand() % 100 == 0) { // ~1% chance every frame
    throw_projectile(projectiles, MAX_PROJECTILES, box.rect);
}

	update(&box, &obs);

         
	update_projectiles(projectiles, MAX_PROJECTILES, &obs);
	draw(screen, &box);
	draw_obstacle(screen, &obs);

	draw_projectiles(screen, projectiles, MAX_PROJECTILES);

	 update_sprite(&enemy);
	 // In your game loop:
int sprite_x = box.rect.x + (box.rect.w - FRAME_WIDTH)/2;

// Handle wrapping for the sprite position
if (sprite_x + FRAME_WIDTH < 0) {
    sprite_x = SCREEN_WIDTH + (sprite_x + FRAME_WIDTH);
}
else if (sprite_x > SCREEN_WIDTH) {
    sprite_x = -FRAME_WIDTH + (sprite_x - SCREEN_WIDTH);
}

int sprite_y = box.rect.y + (box.rect.h - FRAME_HEIGHT);
draw_sprite(screen, &enemy, &box, sprite_x, sprite_y);


        SDL_Flip(screen); 
        SDL_Delay(16);
    }
 
 
	SDL_FreeSurface(box.surface);
    for (int i = 0; i < MAX_PROJECTILES; i++)
        SDL_FreeSurface(projectiles[i].surface);
	 
   free_sprite(&enemy);
	IMG_Quit();
    SDL_Quit();
    return 0;
}

 

