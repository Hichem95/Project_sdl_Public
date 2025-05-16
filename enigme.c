#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_LINES 10 // Nombre maximum de lignes pour la question

// Structure pour une énigme
typedef struct {
    char question[256];
    char reponse1[100];
    char reponse2[100];
    char reponse3[100];
    int correct_answer; // 1, 2 ou 3
    SDL_Surface *surf_question[MAX_LINES]; // Tableau pour les lignes de la question
    int num_lines; // Nombre de lignes pour la question
    SDL_Surface *surf_rep1;
    SDL_Surface *surf_rep2;
    SDL_Surface *surf_rep3;
    SDL_Rect pos_question[MAX_LINES]; // Positions pour chaque ligne de la question
    SDL_Rect pos_rep1;
    SDL_Rect pos_rep2;
    SDL_Rect pos_rep3;
    int used; // 1 si déjà utilisée, 0 sinon
} Enigme;

// Fonction pour charger une image avec gestion d'erreur
SDL_Surface* load_image(const char* path) {
    SDL_Surface* image = IMG_Load(path);
    if (!image) {
        printf("Erreur de chargement de l'image %s : %s\n", path, IMG_GetError());
        return NULL;
    }
    return image;
}

// Fonction pour jouer un son avec gestion d'erreur
void play_sound(Mix_Chunk* sound) {
    if (sound) {
        Mix_PlayChannel(-1, sound, 0);
    }
}

// Fonction pour découper le texte en plusieurs lignes selon une largeur maximale
int split_text_to_fit_width(const char* text, TTF_Font* font, int max_width, char lines[][256], int max_lines) {
    int num_lines = 0;
    char buffer[256];
    strcpy(buffer, text);
    char* word = strtok(buffer, " ");
    char current_line[256] = "";
    int line_width = 0;

    while (word && num_lines < max_lines) {
        int word_width, word_height;
        TTF_SizeText(font, word, &word_width, &word_height);

        // Ajouter un espace avant le mot (sauf pour le premier mot de la ligne)
        char test_line[256];
        strcpy(test_line, current_line);
        if (strlen(test_line) > 0) {
            strcat(test_line, " ");
        }
        strcat(test_line, word);

        // Calculer la largeur de la ligne avec le nouveau mot
        TTF_SizeText(font, test_line, &line_width, &word_height);

        if (line_width <= max_width) {
            strcpy(current_line, test_line);
        } else {
            // La ligne est trop longue, sauvegarder la ligne actuelle et passer à la suivante
            strcpy(lines[num_lines], current_line);
            num_lines++;
            strcpy(current_line, word);
        }

        word = strtok(NULL, " ");
    }

    // Ajouter la dernière ligne si elle n'est pas vide
    if (strlen(current_line) > 0 && num_lines < max_lines) {
        strcpy(lines[num_lines], current_line);
        num_lines++;
    }

    return num_lines;
}

// Fonction pour charger les énigmes depuis le fichier
int load_enigmes(Enigme* enigmes, const char* filename, TTF_Font* font, SDL_Color color) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        return 0;
    }

    int count = 0;
    char line[512];
    while (fgets(line, sizeof(line), file) && count < 10) {
        // Supprimer le \n de la fin
        line[strcspn(line, "\n")] = 0;

        // Découper la ligne
        char* token = strtok(line, "|");
        if (token) strncpy(enigmes[count].question, token, sizeof(enigmes[count].question));
        token = strtok(NULL, "|");
        if (token) strncpy(enigmes[count].reponse1, token, sizeof(enigmes[count].reponse1));
        token = strtok(NULL, "|");
        if (token) strncpy(enigmes[count].reponse2, token, sizeof(enigmes[count].reponse2));
        token = strtok(NULL, "|");
        if (token) strncpy(enigmes[count].reponse3, token, sizeof(enigmes[count].reponse3));
        token = strtok(NULL, "|");
        if (token) enigmes[count].correct_answer = atoi(token);

        // Découper la question en plusieurs lignes si nécessaire (largeur max 400 pixels)
        char question_lines[MAX_LINES][256];
        enigmes[count].num_lines = split_text_to_fit_width(enigmes[count].question, font, 400, question_lines, MAX_LINES);

        // Créer une surface pour chaque ligne de la question
        for (int i = 0; i < enigmes[count].num_lines; i++) {
            enigmes[count].surf_question[i] = TTF_RenderText_Solid(font, question_lines[i], color);
            if (!enigmes[count].surf_question[i]) {
                printf("Erreur de rendu du texte (question ligne %d) : %s\n", i, TTF_GetError());
                fclose(file);
                return 0;
            }
            // Positionner chaque ligne (empilées verticalement)
            enigmes[count].pos_question[i].x = 200; // Coin supérieur gauche du cadre beige
            enigmes[count].pos_question[i].y = 150 + i * 30; // 30 pixels d'espacement entre les lignes
        }

        // Créer les surfaces pour les réponses
        enigmes[count].surf_rep1 = TTF_RenderText_Solid(font, enigmes[count].reponse1, color);
        enigmes[count].surf_rep2 = TTF_RenderText_Solid(font, enigmes[count].reponse2, color);
        enigmes[count].surf_rep3 = TTF_RenderText_Solid(font, enigmes[count].reponse3, color);

        // Vérifier si les surfaces des réponses ont été créées correctement
        if (!enigmes[count].surf_rep1 || !enigmes[count].surf_rep2 || !enigmes[count].surf_rep3) {
            printf("Erreur de rendu du texte (réponses) : %s\n", TTF_GetError());
            fclose(file);
            return 0;
        }

        // Initialiser les positions des réponses
        enigmes[count].pos_rep1.x = 285;
        enigmes[count].pos_rep1.y = 410;
        enigmes[count].pos_rep2.x = 285;
        enigmes[count].pos_rep2.y = 460;
        enigmes[count].pos_rep3.x = 285;
        enigmes[count].pos_rep3.y = 510;

        enigmes[count].used = 0;
        count++;
    }

    fclose(file);
    return count;
}

// Fonction pour sélectionner une énigme aléatoire non utilisée
Enigme* get_random_enigme(Enigme* enigmes, int count) {
    int remaining = 0;
    for (int i = 0; i < count; i++) {
        if (!enigmes[i].used) remaining++;
    }

    if (remaining == 0) return NULL; // Plus d'énigmes disponibles

    int idx = rand() % remaining;
    int current = 0;
    for (int i = 0; i < count; i++) {
        if (!enigmes[i].used) {
            if (current == idx) {
                enigmes[i].used = 1;
                return &enigmes[i];
            }
            current++;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        printf("Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() < 0) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }

    // Initialisation de SDL_mixer (optionnelle)
    Mix_Chunk* click = NULL;
    Mix_Music* music = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Erreur d'initialisation de SDL_mixer : %s\n", Mix_GetError());
        // Continuer sans sons
    }
    else {
        // Charger les sons (optionnel)
        click = Mix_LoadWAV("/home/khalil/Bureau/enigme/click.wav");
        music = Mix_LoadMUS("/home/khalil/Bureau/enigme/son.wav");
        if (!click) {
            printf("Erreur de chargement de click.wav : %s\n", Mix_GetError());
        }
        if (!music) {
            printf("Erreur de chargement de son.wav : %s\n", Mix_GetError());
        }
    }

    // Créer la fenêtre
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        return 1;
    }

    // Charger les fonds
    SDL_Surface* bg1 = load_image("/home/khalil/Bureau/enigme/bg1.png");
    SDL_Surface* bg2 = load_image("/home/khalil/Bureau/enigme/bg2.png");
    if (!bg1 || !bg2) {
        SDL_Quit();
        return 1;
    }

    // Charger les boutons
    SDL_Surface* bq = load_image("/home/khalil/Bureau/enigme/bq.png");
    SDL_Surface* bq_h = load_image("/home/khalil/Bureau/enigme/bq_h.png");
    SDL_Surface* bp = load_image("/home/khalil/Bureau/enigme/bp.png");
    SDL_Surface* bp_h = load_image("/home/khalil/Bureau/enigme/bp_h.png");
    SDL_Surface* b1 = load_image("/home/khalil/Bureau/enigme/b1.png");
    SDL_Surface* b1_h = load_image("/home/khalil/Bureau/enigme/b1_h.png");
    SDL_Surface* b2 = load_image("/home/khalil/Bureau/enigme/b2.png");
    SDL_Surface* b2_h = load_image("/home/khalil/Bureau/enigme/b2_h.png");
    SDL_Surface* b3 = load_image("/home/khalil/Bureau/enigme/b3.png");
    SDL_Surface* b3_h = load_image("/home/khalil/Bureau/enigme/b3_h.png");
    if (!bq || !bq_h || !bp || !bp_h || !b1 || !b1_h || !b2 || !b2_h || !b3 || !b3_h) {
        SDL_Quit();
        return 1;
    }

    // Charger la police avec une taille réduite (24 au lieu de 28)
    TTF_Font* font = TTF_OpenFont("/home/khalil/Bureau/enigme/alagard.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return 1;
    }

    // Couleur du texte (44403c : R=68, G=64, B=60)
    SDL_Color text_color = {68, 64, 60};

    // Charger les énigmes
    Enigme enigmes[10];
    int enigme_count = load_enigmes(enigmes, "/home/khalil/Bureau/enigme/questions.txt", font, text_color);
    if (enigme_count == 0) {
        printf("Aucune énigme chargée\n");
        return 1;
    }

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Positions des boutons (à ajuster selon bg1.png et bg2.png)
    SDL_Rect pos_bq = {300, 200, 0, 0}; // Bouton Quiz
    SDL_Rect pos_bp = {300, 300, 0, 0}; // Bouton Puzzle
    SDL_Rect pos_b1 = {270, 400, 0, 0}; // Réponse 1
    SDL_Rect pos_b2 = {270, 450, 0, 0}; // Réponse 2
    SDL_Rect pos_b3 = {270, 500, 0, 0}; // Réponse 3

    // Position de la barre de progression (chronomètre), juste en dessous du score
    SDL_Rect timer_bar = {SCREEN_WIDTH - 160, 50, 150, 20}; // x=640, y=50, largeur=150, hauteur=20
    Uint32 start_time = 0;
    int time_left = 150; // 150px = 15 secondes (1 seconde = 10px, ajusté pour la nouvelle largeur)

    // Position du score (en haut à droite)
    SDL_Rect pos_score = {SCREEN_WIDTH - 150, 10, 0, 0};
    // Rectangle de fond pour le score (légèrement plus grand que le texte)
    SDL_Rect score_background = {SCREEN_WIDTH - 160, 5, 150, 40};

    // Surface pour le score
    char score_text[32];
    sprintf(score_text, "Score: %d", 100); // Score initial
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text, text_color);

    // État du jeu
    int game_state = 0; // 0: Menu, 1: Quiz, 2: Game Over, 3: Winner
    int score = 100;
    Enigme* current_enigme = NULL;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (game_state == 0) { // Menu
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                // Détecter survol des boutons
                int bq_hovered = (mouse_x >= pos_bq.x && mouse_x <= pos_bq.x + bq->w &&
                                  mouse_y >= pos_bq.y && mouse_y <= pos_bq.y + bq->h);
                int bp_hovered = (mouse_x >= pos_bp.x && mouse_x <= pos_bp.x + bp->w &&
                                  mouse_y >= pos_bp.y && mouse_y <= pos_bp.y + bp->h);

                // Détecter clic
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (bq_hovered) {
                        play_sound(click);
                        if (music) Mix_PlayMusic(music, -1);
                        game_state = 1;
                        current_enigme = get_random_enigme(enigmes, enigme_count);
                        if (current_enigme) {
                            start_time = SDL_GetTicks();
                            time_left = 150;
                        } else {
                            // Si aucune énigme n'est disponible au démarrage, passer directement à l'état approprié
                            if (score > 0) {
                                game_state = 3;
                            } else {
                                game_state = 2;
                            }
                        }
                    }
                    else if (bp_hovered) {
                        play_sound(click);
                        int result = system("./puzzle");
                        if (result != 0) {
                            printf("Erreur : Impossible d'exécuter le puzzle. Assurez-vous que l'exécutable 'puzzle' existe.\n");
                        }
                    }
                }

                // Afficher le menu
                SDL_BlitSurface(bg1, NULL, screen, NULL);
                SDL_BlitSurface(bq_hovered ? bq_h : bq, NULL, screen, &pos_bq);
                SDL_BlitSurface(bp_hovered ? bp_h : bp, NULL, screen, &pos_bp);
                SDL_Flip(screen);
            }
            else if (game_state == 1) { // Quiz
                if (!current_enigme) {
                    // Si current_enigme est NULL, passer directement à l'état approprié
                    if (score > 0) {
                        game_state = 3;
                    } else {
                        game_state = 2;
                    }
                    continue; // Passer au prochain cycle de la boucle
                }

                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                // Détecter survol des boutons de réponse
                int b1_hovered = (mouse_x >= pos_b1.x && mouse_x <= pos_b1.x + b1->w &&
                                  mouse_y >= pos_b1.y && mouse_y <= pos_b1.y + b1->h);
                int b2_hovered = (mouse_x >= pos_b2.x && mouse_x <= pos_b2.x + b2->w &&
                                  mouse_y >= pos_b2.y && mouse_y <= pos_b2.y + b2->h);
                int b3_hovered = (mouse_x >= pos_b3.x && mouse_x <= pos_b3.x + b3->w &&
                                  mouse_y >= pos_b3.y && mouse_y <= pos_b3.y + b3->h);

                // Détecter clic
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    play_sound(click);
                    int answered = 0;
                    if (b1_hovered) {
                        answered = 1;
                        if (current_enigme->correct_answer == 1) score += 20;
                        else score -= 20;
                    }
                    else if (b2_hovered) {
                        answered = 1;
                        if (current_enigme->correct_answer == 2) score += 20;
                        else score -= 20;
                    }
                    else if (b3_hovered) {
                        answered = 1;
                        if (current_enigme->correct_answer == 3) score += 20;
                        else score -= 20;
                    }

                    if (answered) {
                        // Mettre à jour le score affiché
                        SDL_FreeSurface(score_surface);
                        sprintf(score_text, "Score: %d", score);
                        score_surface = TTF_RenderText_Solid(font, score_text, text_color);

                        if (score <= 0) {
                            game_state = 2;
                            continue;
                        }

                        current_enigme = get_random_enigme(enigmes, enigme_count);
                        if (current_enigme == NULL) {
                            if (score > 0) {
                                game_state = 3;
                            } else {
                                game_state = 2;
                            }
                            continue;
                        }

                        start_time = SDL_GetTicks();
                        time_left = 150;
                    }
                }

                // Gérer le chronomètre
                Uint32 current_time = SDL_GetTicks();
                float elapsed = (current_time - start_time) / 1000.0f; // Temps écoulé en secondes
                time_left = 150 - (elapsed * 10); // 1 seconde = 10px (ajusté pour la nouvelle largeur)
                if (time_left <= 0) {
                    score -= 20;
                    // Mettre à jour le score affiché
                    SDL_FreeSurface(score_surface);
                    sprintf(score_text, "Score: %d", score);
                    score_surface = TTF_RenderText_Solid(font, score_text, text_color);

                    if (score <= 0) {
                        game_state = 2;
                        continue;
                    }

                    current_enigme = get_random_enigme(enigmes, enigme_count);
                    if (current_enigme == NULL) {
                        if (score > 0) {
                            game_state = 3;
                        } else {
                            game_state = 2;
                        }
                        continue;
                    }

                    start_time = SDL_GetTicks();
                    time_left = 150;
                }

                // Afficher le quiz (seulement si current_enigme est valide)
                SDL_BlitSurface(bg2, NULL, screen, NULL);
                // Afficher chaque ligne de la question
                for (int i = 0; i < current_enigme->num_lines; i++) {
                    SDL_BlitSurface(current_enigme->surf_question[i], NULL, screen, &current_enigme->pos_question[i]);
                }
                SDL_BlitSurface(b1_hovered ? b1_h : b1, NULL, screen, &pos_b1);
                SDL_BlitSurface(b2_hovered ? b2_h : b2, NULL, screen, &pos_b2);
                SDL_BlitSurface(b3_hovered ? b3_h : b3, NULL, screen, &pos_b3);
                SDL_BlitSurface(current_enigme->surf_rep1, NULL, screen, &current_enigme->pos_rep1);
                SDL_BlitSurface(current_enigme->surf_rep2, NULL, screen, &current_enigme->pos_rep2);
                SDL_BlitSurface(current_enigme->surf_rep3, NULL, screen, &current_enigme->pos_rep3);

                // Afficher le chronomètre (barre de progression)
                SDL_Rect filled_bar = timer_bar;
                filled_bar.w = time_left > 0 ? time_left : 0;
                SDL_FillRect(screen, &filled_bar, SDL_MapRGB(screen->format, 0, 255, 0)); // Vert

                // Afficher le rectangle de fond pour le score (beige clair : R=245, G=222, B=179)
                SDL_FillRect(screen, &score_background, SDL_MapRGB(screen->format, 245, 222, 179));
                // Afficher le score
                SDL_BlitSurface(score_surface, NULL, screen, &pos_score);

                SDL_Flip(screen);
            }
            else if (game_state == 2) { // Game Over
                SDL_Surface* game_over = TTF_RenderText_Solid(font, "GAME OVER", text_color);
                SDL_Rect pos_game_over = {350, 200, 0, 0}; // position game over
                SDL_BlitSurface(bg2, NULL, screen, NULL);
                SDL_BlitSurface(game_over, NULL, screen, &pos_game_over);
                // Afficher le rectangle de fond pour le score final (beige clair)
                SDL_FillRect(screen, &score_background, SDL_MapRGB(screen->format, 245, 222, 179));
                // Afficher le score final
                SDL_BlitSurface(score_surface, NULL, screen, &pos_score);
                SDL_Flip(screen);

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }

                SDL_FreeSurface(game_over);
            }
            else if (game_state == 3) { // Winner
                SDL_Surface* winner = TTF_RenderText_Solid(font, "Winner", text_color);
                SDL_Rect pos_winner = {350, 200, 0, 0}; // position winner
                SDL_BlitSurface(bg2, NULL, screen, NULL);
                SDL_BlitSurface(winner, NULL, screen, &pos_winner);
                // Afficher le rectangle de fond pour le score final (beige clair)
                SDL_FillRect(screen, &score_background, SDL_MapRGB(screen->format, 245, 222, 179));
                // Afficher le score final
                SDL_BlitSurface(score_surface, NULL, screen, &pos_score);
                SDL_Flip(screen);

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }

                SDL_FreeSurface(winner);
            }
        }
    }

    // Libérer les ressources
    for (int i = 0; i < enigme_count; i++) {
        for (int j = 0; j < enigmes[i].num_lines; j++) {
            SDL_FreeSurface(enigmes[i].surf_question[j]);
        }
        SDL_FreeSurface(enigmes[i].surf_rep1);
        SDL_FreeSurface(enigmes[i].surf_rep2);
        SDL_FreeSurface(enigmes[i].surf_rep3);
    }
    SDL_FreeSurface(bg1);
    SDL_FreeSurface(bg2);
    SDL_FreeSurface(bq);
    SDL_FreeSurface(bq_h);
    SDL_FreeSurface(bp);
    SDL_FreeSurface(bp_h);
    SDL_FreeSurface(b1);
    SDL_FreeSurface(b1_h);
    SDL_FreeSurface(b2);
    SDL_FreeSurface(b2_h);
    SDL_FreeSurface(b3);
    SDL_FreeSurface(b3_h);
    SDL_FreeSurface(score_surface);
    if (click) Mix_FreeChunk(click);
    if (music) Mix_FreeMusic(music);
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
