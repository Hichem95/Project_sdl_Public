#include "header.h"



//chargement images
void chargement_images(SDL_Surface **S, char *path, int index) {
    S[index] = IMG_Load(path);
    if (!S[index]) {
        printf("Erreur chargement image %s: %s\n", path, IMG_GetError());
    }
}


//intialiser joueur 
int init_joueur(joueur *j, int x, int y, int xS, int yS, int yV) {
    j->pos.x = x;
    j->pos.y = y;
    j->pos.w = j->image->w;
    j->pos.h = j->image->h;
    j->life_count = 5;
    j->score_count = 0;
    j->posS.x = xS;
    j->posS.y = yS;

    for (int i = 0; i < j->life_count; i++) {
        j->vie[i] =  IMG_Load("sprite/coeur.png");
        if (!j->vie[i]) {
            printf("Erreur: j.vie[%d] est NULL\n", i);
            for (int k = 0; k < i; k++) SDL_FreeSurface(j->vie[k]);
            SDL_FreeSurface(j->image);
            SDL_Quit();
            return 1;
        }
        j->posV[i].x = 10 + i * 55;
        j->posV[i].y = yV;
        j->posV[i].w = j->vie[i]->w;
        j->posV[i].h = j->vie[i]->h;
    }
    return 0;
}



//vie 
void gestion_vie(joueur *j, int collision, Uint32 *last_hit_time, int is_attacking) {
    if (collision && !is_attacking) {
        Uint32 now = SDL_GetTicks();
        if (now - *last_hit_time > 1000 && j->life_count > 0) {
            j->life_count--;
            *last_hit_time = now;
        }
    }
}
//score gestion et creation de surface a partir du texte 
void gestion_score(joueur *j, Uint32 start_time, TTF_Font *font, int hit_score) {
    if (j->life_count > 0) {
        j->score_count = (SDL_GetTicks() - start_time) / 100;
        j->score_count += hit_score;
    }

    SDL_Color white = {255, 255, 255};
    char scoreText[20];
    sprintf(scoreText, "Score: %d", j->score_count);
    if (j->score) SDL_FreeSurface(j->score);
    j->score = TTF_RenderText_Solid(font, scoreText, white);
}



// rebondir lorsque on touche l'obstacle  
void rebond_colli(joueur *j, minibox *box, int *jump, float *vy) {
    int player_center_x = j->pos.x + (j->pos.w / 2);
    int enemy_center_x = box->rect.x + (box->rect.w / 2);

    if (j->pos.x + j->pos.w >= box->rect.x && j->pos.x + j->pos.w <= enemy_center_x && j->pos.y + j->pos.h > box->rect.y + 5)
        j->pos.x -= 50;
    else if (j->pos.x <= box->rect.x + box->rect.w && j->pos.x >= enemy_center_x && j->pos.y + j->pos.h > box->rect.y + 5)
        j->pos.x += 50;
    else {
        j->pos.y = box->rect.y - j->pos.h;
        *vy = 0;
        *jump = 0;
    }
}


// voir si le joueur peut attaquer
int can_attack(SDL_Rect attacker, SDL_Rect target, int range, int facing) {
    int direction = (attacker.x < target.x) ? FACING_RIGHT : FACING_LEFT;
    return abs(attacker.x - target.x) <= range && direction == facing && check_collision(attacker, target);
}


// ajout de score lors de l'attaque, ne fonctionne plus depuis que j'ai ajouté l'animation
void handle_attack(joueur *j, minibox *box, int *attack, Uint32 *last_hit_time1, int *hit_score) {
    if (*attack && j->life_count > 0) {
        Uint32 now = SDL_GetTicks();
        if (now - *last_hit_time1 > 500) {
            if (can_attack(j->pos, box->rect, 60, j->facing)) {
                if (box->sante > 0) box->sante--;
                *hit_score += 100;
                *last_hit_time1 = now;
            }
        }
    }
}

// deplacement, inclus aussi l'animation
void deplacer_joueur(joueur *j, SDL_Surface *screen, int right, int left, int up, int attack, int a, int *jump, float *vx, float *vy) {
    if (j->life_count <= 0) return;

    if (right) j->facing = FACING_RIGHT;
    if (left) j->facing = FACING_LEFT;

    int animation_type;
    if (attack) animation_type = (j->facing == FACING_RIGHT) ? 6 : 7;
    else if (*jump) animation_type = (j->facing == FACING_RIGHT) ? 4 : 5;
    else if (right || left) animation_type = (j->facing == FACING_RIGHT) ? 2 : 3;
    else animation_type = (j->facing == FACING_RIGHT) ? 0 : 1;

    animate_player(j, animation_type);

    j->x_vit = *vx;
    j->y_vit = *vy;

    if (right) j->pos.x += j->x_vit;
    if (left) j->pos.x -= j->x_vit;

    if (a && *vx < 15) *vx += acc;
    else if (!a && *vx > 8) *vx -= 0.1;
    if (*vx < 8) *vx = 8;

    if (up && *jump == 0) {
        *vy = -15;
        *jump = 1;
    }

    *vy += 0.5;
    j->pos.y += *vy;
    if (j->pos.y >= 430) {
        j->pos.y = 430;
        *vy = 0;
        *jump = 0;
    }
}


// gestion de l'animation
void animate_player(joueur *j, int new_state) {
    Uint32 current_time = SDL_GetTicks();

    if (j->animation_state != new_state) {
        j->animation_state = new_state;
        j->current_frame = 0;
        j->frame_timer = current_time;
    }

    if (new_state == 0 || new_state == 1) {
        j->current_frame = 0;
    } else {
        int frame_delay = 120;
        if (new_state == 4 || new_state == 5 || new_state == 6 || new_state == 7) {
            if (j->current_frame == 2 && current_time - j->frame_timer < 300) return;
            else if (current_time - j->frame_timer > frame_delay) {
                j->current_frame = (j->current_frame + 1) % 5;
                j->frame_timer = current_time;
            }
        } else if (current_time - j->frame_timer > frame_delay) {
            j->current_frame = (j->current_frame + 1) % 5;
            j->frame_timer = current_time;
        }
    }

    j->frame.x = j->current_frame * 234;
    j->frame.y = 0;
    j->frame.w = 234;
    j->frame.h = 220;
}
