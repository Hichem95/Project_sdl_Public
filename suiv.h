#ifndef suuiv_H
#define suiv_H
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_image.h>

#define FRAME_WIDTH 68
#define FRAME_HEIGHT 128
#define TOTAL_FRAMES 4

 

typedef struct {
    SDL_Surface* spritesheet;
    SDL_Rect clips[TOTAL_FRAMES];
    int current_frame;
    Uint32 last_frame_time;
    Uint32 frame_delay;
	 

} AnimatedSprite;


typedef struct{
	SDL_Surface *surface;
    	SDL_Rect rect;
     
    int xvitesse, yvitesse;
    int xacc, yacc;
	int accident;
	int sante;

	 
     

}minibox;


typedef struct {
    SDL_Surface *surface;
    SDL_Rect rect;
    int active; // 1 if on screen
    int speed;
    int vx;
} projectile;


typedef struct {
    SDL_Rect rect;
    int health;
    int visible;
    SDL_Surface* surface;
} obstacle;



 


SDL_Surface* create_color_surface(int w, int h, Uint8 r, Uint8 g, Uint8 b);
void init_obstacle(obstacle *obs, int x, int y, int w, int h, int health);
void init_box(minibox *box);
void update(minibox *box, obstacle *obs);
void draw(SDL_Surface* screen, minibox *box);
void init_projectiles(projectile *p, int count);
void update_projectiles(projectile *p, int count, obstacle *obs);
void draw_projectiles(SDL_Surface* screen, projectile *p, int count);
void throw_projectile(projectile *p, int count, SDL_Rect start_pos);
void draw_obstacle(SDL_Surface *screen, obstacle *obs);
void update_obstacle_color(obstacle *obs);
int init_sprite(AnimatedSprite *sprite, const char* filepath);
void update_sprite(AnimatedSprite* sprite);
void draw_sprite(SDL_Surface* screen, AnimatedSprite* sprite,minibox *box, int x, int y);
void free_sprite(AnimatedSprite* sprite);


#endif 

