#ifndef HEADER_H_
#define HEADER_H_
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
 

#define acc 0.5
#define fr 0.1
#define FACING_RIGHT 0
#define FACING_LEFT 1
#define FRAME_DELAY 150
#define FRAME_WIDTH 125
#define FRAME_HEIGHT 220
#define TOTAL_FRAMES 4

typedef enum { PATROL, CHASE } EnemyState;



typedef struct {
    SDL_Surface *sprites[8];
    SDL_Surface *image;
    SDL_Surface *vie[10];
    SDL_Surface *score;

    SDL_Rect frame;
    int current_frame;
    int animation_state;
    Uint32 frame_timer;
    int facing;

    SDL_Rect posV[10];
    SDL_Rect posS;
    SDL_Rect pos;
    float x_vit, y_vit;
    int life_count;
    int score_count;
} joueur;


//structure enemy:
typedef struct {
    SDL_Surface* spritesheet;
    SDL_Rect clips[TOTAL_FRAMES];
    int current_frame;
    Uint32 last_frame_time;
    Uint32 frame_delay;
    int facingLeft;  // 0 = right, 1 = left
 
	 

} AnimatedSprite;


typedef struct{
	SDL_Surface *surface;
    	SDL_Rect rect;
     
    int xvitesse, yvitesse;
    int xacc, yacc;
	int accident;
	int sante;
	EnemyState state;    // current behavior
	int patrol_dir; // +1 = moving right, –1 = moving left
	int facingLeft;  // 0 = right, 1 = left
    	AnimatedSprite spriteRight;
    	AnimatedSprite spriteLeft; 
	int chase;
    	                
} minibox;





typedef struct {
    SDL_Surface *surface;
    SDL_Rect rect;
    int active; // 1 if on screen
    int speed;
    int vx;
} projectile;





//menu
int check_hover(SDL_Rect button, int x, int y);
void chargement_images2(SDL_Surface **S, char *path  );
int play_menu();
int option_menu(Mix_Music **music);
int joueur_menu();
int joueur_mono(int selectav1, int selectav2, int selectin1, int selectin2);
int joueur_multiple( int  selectav1, int selectav2, int selectin1, int selectin2);
int levelcomplete(void);
int gameover(void);
void recalculate_button_positions();

// Utility & Init
void chargement_images(SDL_Surface **S, char *path, int index);
int init_joueur(joueur *j, int x, int y, int xS, int yS, int yV);

// Core Mechanics
void deplacer_joueur2(joueur *j, SDL_Surface *screen, int right, int left, int up, int attack, int a, int *jump, float *vx, float *vy);
void deplacer_joueur(joueur *j, SDL_Surface *screen, int right, int left, int up, int attack, int a, int *jump, float *vx, float *vy);
void gestion_vie(joueur *j, int collision, Uint32 *last_hit_time, int is_attacking);
void gestion_score(joueur *j, Uint32 start_time, TTF_Font *font, int hit_score);
void rebond_colli(joueur *j, minibox *box, int *jump, float *vy);

// Combat
void handle_attack(joueur *j, minibox *box, int *attack, Uint32 *last_hit_time1, int *hit_score);
int can_attack(SDL_Rect attacker, SDL_Rect target, int range, int facing);

// Animation
void animate_player(joueur *j, int new_state);



//les fonctions enemmi:
SDL_Surface* create_color_surface(int w, int h, Uint8 r, Uint8 g, Uint8 b);
void init_box(minibox *box);
void update(minibox *box, joueur *j,int attack1);
void draw(SDL_Surface* screen, minibox *box);
void init_projectiles(projectile *p, int count);
void update_projectiles(projectile *p, int count, joueur *j);
void draw_projectiles(SDL_Surface* screen, projectile *p, int count);
void throw_projectile(projectile *p, int count, SDL_Rect start_pos);
int init_sprite(AnimatedSprite *sprite, const char* filepath);
void update_sprite(AnimatedSprite* sprite);
void draw_sprite(SDL_Surface* screen, AnimatedSprite* sprite,minibox *box, int x, int y);
void free_sprite(AnimatedSprite* sprite);
void draw_health_bar(SDL_Surface* screen, minibox *box);
int check_collision(SDL_Rect a, SDL_Rect b);
 

#endif

