#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

SDL_Surface *screen1 = NULL;
SDL_Surface *bg1 = NULL, *bg2 = NULL;
SDL_Surface *valid_h = NULL, *return_h = NULL, *quit_h = NULL;

SDL_Rect valid_rect = {701, 410, 84, 76};
SDL_Rect return_rect = {1121, 89, 51, 51};
SDL_Rect quit_rect = {1121, 29, 51, 51};
SDL_Rect text_rect = {430, 415, 209, 62};

SDL_Rect star1_rect = {200, 150, 50, 50};
SDL_Rect star2_rect = {400, 250, 50, 50};
SDL_Rect star3_rect = {600, 350, 50, 50};

Mix_Music *backgroundMusic = NULL;
Mix_Chunk *buttonSound = NULL;
TTF_Font *font = NULL;

char pseudo[256] = "";
int pseudoValidated = 0;
int startTyping = 0;

typedef struct {
    char name[256];
    int score;
} PlayerScore;

PlayerScore scores[3];

// 💡 Scores personnalisables
int scoreJoueur1 = 22;
int scoreJoueur2 = 73;
int scoreJoueur3 = 65;

void sauvegarderScores(PlayerScore scores[], int nb) {
    FILE *f = fopen("scores.dat", "wb");
    if (f) {
        fwrite(scores, sizeof(PlayerScore), nb, f);
        fclose(f);
    }
}

void sauvegarderScoresTxt(PlayerScore scores[], int nb) {
    FILE *f = fopen("scores.txt", "w");
    if (f) {
        for (int i = 0; i < nb; i++) {
            fprintf(f, "%s : %d pts\n", scores[i].name, scores[i].score);
        }
        fclose(f);
    }
}

int chargerScores(PlayerScore scores[], int max) {
    FILE *f = fopen("scores.dat", "rb");
    if (f) {
        int nb = fread(scores, sizeof(PlayerScore), max, f);
        fclose(f);
        return nb;
    }
    return 0;
}

void trierScores(PlayerScore scores[], int nb) {
    for (int i = 0; i < nb - 1; i++) {
        for (int j = i + 1; j < nb; j++) {
            if (scores[i].score < scores[j].score) {
                PlayerScore tmp = scores[i];
                scores[i] = scores[j];
                scores[j] = tmp;
            }
        }
    }
}

int isMouseOver(SDL_Rect pos, int x, int y) {
    return (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h);
}

void renderText(char* text, SDL_Surface* surface, int x, int y) {
    SDL_Color color = {255, 255, 255};
    SDL_Surface* tmp = TTF_RenderText_Blended(font, text, color);
    if (tmp) {
        SDL_Rect dst = {x, y};
        SDL_BlitSurface(tmp, NULL, surface, &dst);
        SDL_FreeSurface(tmp);
    }
}

void renderScoreManuel(SDL_Surface* surface, PlayerScore player, int x, int y) {
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "🏆 %s - %d pts", player.name, player.score);
    renderText(buffer, surface, x, y);
}

void generateRandomScores() {
    snprintf(scores[0].name, sizeof(scores[0].name), "Joueur 1");
    scores[0].score = scoreJoueur1;

    snprintf(scores[1].name, sizeof(scores[1].name), "Joueur 2");
    scores[1].score = scoreJoueur2;

    snprintf(scores[2].name, sizeof(scores[2].name), "Joueur 3");
    scores[2].score = scoreJoueur3;

    trierScores(scores, 3);
    sauvegarderScoresTxt(scores, 3);
}

int meilleur(joueur j) {
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    font = TTF_OpenFont("alagard.ttf", 24);
    backgroundMusic = Mix_LoadMUS("son/music.mp3");
    buttonSound = Mix_LoadWAV("son/son.wav");
    Mix_PlayMusic(backgroundMusic, -1);

    screen1 = SDL_SetVideoMode(1204, 800, 32, SDL_SWSURFACE);
    bg1 = IMG_Load("yahya/bg1.png");
    bg2 = IMG_Load("yahya/bg2.png");
    valid_h = IMG_Load("yahya/valid_h.png");
    return_h = IMG_Load("yahya/return_h.png");
    quit_h = IMG_Load("yahya/quit_h.png");

    PlayerScore allScores[10];
    int nbScores = chargerScores(allScores, 10);

    int running = 1, currentBg = 1, mx = 0, my = 0;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEMOTION:
                    mx = event.motion.x;
                    my = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (currentBg == 1) {
                            if (isMouseOver(valid_rect, mx, my)) {
                                Mix_PlayChannel(-1, buttonSound, 0);
                                pseudoValidated = 1;
                                // Chercher si le pseudo existe déjà
                                int found = 0;
                                for (int i = 0; i < nbScores; i++) {
                                    if (strcmp(allScores[i].name, pseudo) == 0) {
                                        allScores[i].score = j.score_count;
                                        found = 1;
                                        break;
                                    }
                                }
                                // Sinon l'ajouter
                                if (!found && nbScores < 10) {
                                    snprintf(allScores[nbScores].name, sizeof(allScores[nbScores].name), "%s", pseudo);
                                    allScores[nbScores].score = j.score_count;
                                    nbScores++;
                                }
                                trierScores(allScores, nbScores);
                                sauvegarderScores(allScores, nbScores);
                                sauvegarderScoresTxt(allScores, nbScores);
                                currentBg = 2;
                            }
                            if (isMouseOver(text_rect, mx, my)) {
                                startTyping = 1;
                            }
                        } else if (currentBg == 2) {
                            if (isMouseOver(return_rect, mx, my) || isMouseOver(quit_rect, mx, my)) {
                                Mix_PlayChannel(-1, buttonSound, 0);
                                running = 0;
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (currentBg == 1) {
                        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(pseudo) > 0) {
                            pseudo[strlen(pseudo) - 1] = '\0';
                        } else if (event.key.keysym.sym == SDLK_RETURN) {
                            // Même traitement que clic sur valider
                            Mix_PlayChannel(-1, buttonSound, 0);
                            pseudoValidated = 1;
                            int found = 0;
                            for (int i = 0; i < nbScores; i++) {
                                if (strcmp(allScores[i].name, pseudo) == 0) {
                                    allScores[i].score = j.score_count;
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found && nbScores < 10) {
                                snprintf(allScores[nbScores].name, sizeof(allScores[nbScores].name), "%s", pseudo);
                                allScores[nbScores].score = j.score_count;
                                nbScores++;
                            }
                            trierScores(allScores, nbScores);
                            sauvegarderScores(allScores, nbScores);
                            sauvegarderScoresTxt(allScores, nbScores);
                            currentBg = 2;
                        } else {
                            if (startTyping && strlen(pseudo) < 12) {
                                char c = event.key.keysym.sym;
                                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ') {
                                    int len = strlen(pseudo);
                                    pseudo[len] = c;
                                    pseudo[len + 1] = '\0';
                                }
                            }
                        }
                    }
                    break;
            }
        }

        SDL_BlitSurface((currentBg == 1 ? bg1 : bg2), NULL, screen1, NULL);

        if (currentBg == 1) {
            if (isMouseOver(valid_rect, mx, my))
                SDL_BlitSurface(valid_h, NULL, screen1, &valid_rect);
            renderText(pseudo, screen1, text_rect.x, text_rect.y);
            renderText("Clique ici pour écrire ton pseudo", screen1, 360, 350);
        } else if (currentBg == 2) {
            if (isMouseOver(return_rect, mx, my)) SDL_BlitSurface(return_h, NULL, screen1, &return_rect);
            if (isMouseOver(quit_rect, mx, my)) SDL_BlitSurface(quit_h, NULL, screen1, &quit_rect);

            for (int i = 0; i < 3 && i < nbScores; i++) {
                renderScoreManuel(screen1, allScores[i], 400, 340 + i * 100);
            }
        }

        SDL_Flip(screen1);
    }

    // Libération sécurisée
    if (bg1) SDL_FreeSurface(bg1);
    if (bg2) SDL_FreeSurface(bg2);
    if (valid_h) SDL_FreeSurface(valid_h);
    if (return_h) SDL_FreeSurface(return_h);
    if (quit_h) SDL_FreeSurface(quit_h);

    if (font) TTF_CloseFont(font);
    
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (buttonSound) Mix_FreeChunk(buttonSound);

    // Quitter dans le bon ordre
    Mix_CloseAudio();   // audio d'abord
    TTF_Quit();         // ensuite ttf
    IMG_Quit();         // ensuite IMG
    SDL_Quit();         // enfin SDL

    return 0;
}

