#include "header.h"


// Attempt to switch to fullscreen at the desktop's current resolution.
// Returns the new screen pointer, or NULL on failure.
static SDL_Surface* enter_fullscreen(SDL_Surface* old_screen, int* is_fullscreen_flag) {
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    if (!info) {
        fprintf(stderr, "SDL_GetVideoInfo failed: %s\n", SDL_GetError());
        return NULL;
    }
    int w = info->current_w;
    int h = info->current_h;
    int bpp = info->vfmt->BitsPerPixel;

    SDL_Surface* newScreen = SDL_SetVideoMode(w, h, bpp,
        SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    if (!newScreen) {
        fprintf(stderr, "Erreur passage plein écran : %s\n", SDL_GetError());
        return NULL;
    }
    *is_fullscreen_flag = 1;
    return newScreen;
}

// Switch back to windowed mode at the stored window dimensions.
// Returns the new screen pointer, or NULL on failure.
static SDL_Surface* exit_fullscreen(int win_w, int win_h, int bpp, int* is_fullscreen_flag) {
    SDL_Surface* newScreen = SDL_SetVideoMode(win_w, win_h, bpp,
        SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!newScreen) {
        fprintf(stderr, "Erreur passage fenêtré : %s\n", SDL_GetError());
        return NULL;
    }
    *is_fullscreen_flag = 0;
    return newScreen;
}


int option_menu(Mix_Music **music){
 printf("Menu d'options lancé !\n");
// Déclaration des variables
SDL_Event event;
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *background_f = NULL;
SDL_Surface *background_m = NULL;
SDL_Surface *background_m_f = NULL;
SDL_Surface *fullscreen_button = NULL;
SDL_Surface *normal_button = NULL;
SDL_Surface *increase_button = NULL;
SDL_Surface *decrease_button = NULL;
SDL_Surface *x_button = NULL;
SDL_Surface *volume_images[6];

int running;
int is_fullscreen;

// Déclaration des rectangles pour les boutons et autres éléments
SDL_Rect x_button_rect;
SDL_Rect fullscreen_btn_rect;
SDL_Rect normal_btn_rect;
SDL_Rect increase_btn_rect;
SDL_Rect decrease_btn_rect;
SDL_Rect destRect;          // Pour le fond secondaire
SDL_Rect destRect_f;          // Pour le fond secondaire enn pleinn ecran
SDL_Rect volume_rect;       // Pour la barre de volume
SDL_Rect textRect;          // Pour le texte "Volume :"
SDL_Rect displayModeTextRect; // Pour le texte "Display Mode :"


// Fonction pour recalculer les positions et dimensions des éléments
void recalculate_button_positions() {
	if(is_fullscreen == 0) {
    // Recalcul des positions des boutons
    x_button_rect.x = (int)(0.94 * screen->w);  // 82% de la largeur
    x_button_rect.y = (int)(0.009 * screen->h);  // 8% de la hauteur
    x_button_rect.w = (int)(30 * (screen->w / 1024.0));  // Ajuster la largeur
    x_button_rect.h = (int)(30 * (screen->h / 768.0));   // Ajuster la hauteur

    fullscreen_btn_rect.x = (int)(0.265 * screen->w);  // 36% de la largeur
    fullscreen_btn_rect.y = (int)(0.6 * screen->h);  // 52% de la hauteur
    fullscreen_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    fullscreen_btn_rect.h = (int)(50 * (screen->h / 768.0));

    normal_btn_rect.x = (int)(0.50 * screen->w);  // 64% de la largeur
    normal_btn_rect.y = (int)(0.6 * screen->h);  // 52% de la hauteur
    normal_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    normal_btn_rect.h = (int)(50 * (screen->h / 768.0));

    increase_btn_rect.x = (int)(0.68 * screen->w);  // 63% de la largeur
    increase_btn_rect.y = (int)(0.39 * screen->h);  // 26% de la hauteur
    increase_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    increase_btn_rect.h = (int)(50 * (screen->h / 768.0));

    decrease_btn_rect.x = (int)(0.265 * screen->w);  // 54% de la largeur
    decrease_btn_rect.y = (int)(0.40 * screen->h);  // 26% de la hauteur
    decrease_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    decrease_btn_rect.h = (int)(50 * (screen->h / 768.0));

    // Recalcul des positions et dimensions des autres éléments
    destRect_f.x = (int)(200 * (screen->w / 1024.0));
    destRect_f.y = (int)(40 * (screen->h / 768.0));
    destRect_f.w = (int)(300 * (screen->w / 1024.0));
    destRect_f.h = (int)(100 * (screen->h / 768.0));

    destRect.x = (int)(190 * (screen->w / 1024.0));
    destRect.y = (int)(150 * (screen->h / 768.0));
    destRect.w = (int)(600 * (screen->w / 1024.0));
    destRect.h = (int)(400 * (screen->h / 768.0));

    volume_rect.x = (int)(320 * (screen->w / 1024.0));
    volume_rect.y = (int)(300 * (screen->h / 768.0));
    volume_rect.w = (int)(50 * (screen->w / 1024.0));
    volume_rect.h = (int)(50 * (screen->h / 768.0));

    textRect.x = (int)(450 * (screen->w / 1024.0));
    textRect.y = (int)(180 * (screen->h / 768.0));
    textRect.w = (int)(100 * (screen->w / 1024.0));  // Largeur approximative
    textRect.h = (int)(30 * (screen->h / 768.0));    // Hauteur approximative

    displayModeTextRect.x = (int)(550 * (screen->w / 1024.0));
    displayModeTextRect.y = (int)(450 * (screen->h / 768.0));
    displayModeTextRect.w = (int)(200 * (screen->w / 1024.0));  // Largeur approximative
    displayModeTextRect.h = (int)(30 * (screen->h / 768.0));    // Hauteur approximative
}
else 
{
    // Recalcul des positions des boutons
    x_button_rect.x = (int)(0.844 * screen->w);  // 82% de la largeur
    x_button_rect.y = (int)(0.226 * screen->h);  // 8% de la hauteur
    x_button_rect.w = (int)(30 * (screen->w / 1024.0));  // Ajuster la largeur
    x_button_rect.h = (int)(30 * (screen->h / 768.0));   // Ajuster la hauteur

    fullscreen_btn_rect.x = (int)(0.4 * screen->w);  // 36% de la largeur
    fullscreen_btn_rect.y = (int)(0.6 * screen->h);  // 52% de la hauteur
    fullscreen_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    fullscreen_btn_rect.h = (int)(50 * (screen->h / 768.0));

    normal_btn_rect.x = (int)(0.7 * screen->w);  // 64% de la largeur
    normal_btn_rect.y = (int)(0.6 * screen->h);  // 52% de la hauteur
    normal_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    normal_btn_rect.h = (int)(50 * (screen->h / 768.0));

    increase_btn_rect.x = (int)(0.63 * screen->w);  // 63% de la largeur
    increase_btn_rect.y = (int)(0.40 * screen->h);  // 26% de la hauteur
    increase_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    increase_btn_rect.h = (int)(50 * (screen->h / 768.0));

    decrease_btn_rect.x = (int)(0.55 * screen->w);  // 54% de la largeur
    decrease_btn_rect.y = (int)(0.40 * screen->h);  // 26% de la hauteur
    decrease_btn_rect.w = (int)(50 * (screen->w / 1024.0));
    decrease_btn_rect.h = (int)(50 * (screen->h / 768.0));

    // Recalcul des positions et dimensions des autres éléments
    destRect_f.x = (int)(200 * (screen->w / 1024.0));
    destRect_f.y = (int)(40 * (screen->h / 768.0));
    destRect_f.w = (int)(200 * (screen->w / 1024.0));
    destRect_f.h = (int)(100 * (screen->h / 768.0));

    destRect.x = (int)(220 * (screen->w / 1024.0));
    destRect.y = (int)(-120 * (screen->h / 768.0));
    destRect.w = (int)(600 * (screen->w / 1024.0));
    destRect.h = (int)(400 * (screen->h / 768.0));

    volume_rect.x = (int)(520 * (screen->w / 1024.0));
    volume_rect.y = (int)(240 * (screen->h / 768.0));
    volume_rect.w = (int)(50 * (screen->w / 1024.0));
    volume_rect.h = (int)(50 * (screen->h / 768.0));

    textRect.x = (int)(560 * (screen->w / 1024.0));
    textRect.y = (int)(190 * (screen->h / 768.0));
    textRect.w = (int)(100 * (screen->w / 1024.0));  // Largeur approximative
    textRect.h = (int)(30 * (screen->h / 768.0));    // Hauteur approximative

    displayModeTextRect.x = (int)(560 * (screen->w / 1024.0));
    displayModeTextRect.y = (int)(400 * (screen->h / 768.0));
    displayModeTextRect.w = (int)(200 * (screen->w / 1024.0));  // Largeur approximative
    displayModeTextRect.h = (int)(30 * (screen->h / 768.0));    // Hauteur approximative
}
}
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation de SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        return 1;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }

    // Chargement de la police Alagard
    TTF_Font *font = TTF_OpenFont("alagard.ttf", 34);  // Taille de police 34
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

    // Couleur du texte (rouge bordeaux)
    SDL_Color textColor = {139, 0, 0};

    // Création de la fenêtre
    screen = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Erreur création écran : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation des positions des boutons
    recalculate_button_positions();

    // Chargement des images
    background = IMG_Load("images/bg.jpg");
    background_f = IMG_Load("images/bg2.jpg");
    if (!background || !background_f) {
        printf("Erreur chargement background : %s\n", IMG_GetError());
        return 1;
    }
    background_m_f = IMG_Load("images/sous_bg2.png");
    background_m = IMG_Load("images/sous_bg.png");
    if (!background_m || !background_m_f) {
        printf("Erreur chargement background_m : %s\n", IMG_GetError());
        return 1;
    }

    fullscreen_button = IMG_Load("images/Screen_mode_Button/FULL.png");
    if (!fullscreen_button) {
        printf("Erreur chargement bouton fullscreen : %s\n", IMG_GetError());
    }

    normal_button = IMG_Load("images/Screen_mode_Button/NORMAL.png");
    if (!normal_button) {
        printf("Erreur chargement bouton normal : %s\n", IMG_GetError());
        return 1;
    }

    increase_button = IMG_Load("images/Buttons/+.png");
    if (!increase_button) {
        printf("Erreur chargement bouton augmenter volume : %s\n", IMG_GetError());
        return 1;
    }

    decrease_button = IMG_Load("images/Buttons/-.png");
    if (!decrease_button) {
        printf("Erreur chargement bouton diminuer volume : %s\n", IMG_GetError());
        return 1;
    }

    x_button = IMG_Load("images/x_button.png");
    if (!x_button) {
        printf("Erreur chargement x button : %s\n", IMG_GetError());
        return 1;
    }

    // Chargement des images de la barre de volume
    char *volume_filenames[] = {
        "images/Sound Bar/Empty.png", "images/Sound Bar/1.png", "images/Sound Bar/2.png", 
        "images/Sound Bar/3.png", "images/Sound Bar/4.png", "images/Sound Bar/full.png"
    };

    for (int i = 0; i < 6; i++) {
        volume_images[i] = IMG_Load(volume_filenames[i]);
        if (!volume_images[i]) {
            printf("Erreur chargement %s : %s\n", volume_filenames[i], IMG_GetError());
            for (int j = 0; j < i; j++) {
                SDL_FreeSurface(volume_images[j]);
            }
            return 1;
        }
    }

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        for (int i = 0; i < 6; i++) {
            SDL_FreeSurface(volume_images[i]);
        }
        return 1;
    }

    // Chargement de la musique
    *music = Mix_LoadMUS("son/music.mp3");
    if (!*music) {
        printf("Erreur chargement musique : %s\n", Mix_GetError());
        Mix_CloseAudio();
        for (int i = 0; i < 6; i++) {
            SDL_FreeSurface(volume_images[i]);
        }
        return 1;
    }
    Mix_PlayMusic(*music, -1); // Jouer en boucle

    // Boucle principale
    running = 1;
    is_fullscreen = 0;
    const int win_w   = screen->w;
    const int win_h   = screen->h;
    const int win_bpp = screen->format->BitsPerPixel;


    while (running) {
        while (SDL_PollEvent(&event)) {
            int x, y;
            SDL_GetMouseState(&x, &y);  // Obtenir les coordonnées de la souris

            // Gestion des événements
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYDOWN:
                    // Gestion des touches clavier
                    switch (event.key.keysym.sym) {
                        case SDLK_f: {
    				SDL_Surface* newScreen = enter_fullscreen(screen, &is_fullscreen);
   				if (newScreen) {
        				screen = newScreen;
        				recalculate_button_positions();
    				}
    			break;
			}
			case SDLK_n: {
    				if (is_fullscreen) {
        				SDL_Surface* newScreen = exit_fullscreen(win_w, win_h, win_bpp, &is_fullscreen);
        				if (newScreen) {
            					screen = newScreen;
            					recalculate_button_positions();
        				}
    				}
    			break;
			}

                        case SDLK_PLUS:  // Augmenter volume
                        case SDLK_KP_PLUS:
                            {
                                int current_volume = Mix_VolumeMusic(-1);
                                if (current_volume < MIX_MAX_VOLUME) {
                                    Mix_VolumeMusic(current_volume + 10);
                                    if (Mix_PausedMusic()) {
                                        Mix_ResumeMusic();
                                    }
                                }
                            }
                            break;
                        case SDLK_MINUS:  // Diminuer volume
                        case SDLK_KP_MINUS:
                            {
                                int current_volume = Mix_VolumeMusic(-1);
                                if (current_volume > 0) {
                                    Mix_VolumeMusic(current_volume - 10);
                                    if (current_volume - 10 <= 0) {
                                        Mix_PauseMusic();
                                    }
                                } else {
                                    Mix_PauseMusic();
                                }
                            }
                            break;
                        case SDLK_ESCAPE:  // Quitter l'application
                            running = 0;
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // Vérification des zones des boutons
                    if (x >= fullscreen_btn_rect.x && x <= fullscreen_btn_rect.x + fullscreen_btn_rect.w && y >= fullscreen_btn_rect.y && y <= fullscreen_btn_rect.y + fullscreen_btn_rect.h) {
    			SDL_Surface* newScreen = enter_fullscreen(screen, &is_fullscreen);
    		    	if (newScreen) {
       			 	screen = newScreen;
        			recalculate_button_positions();
    			}
		    }else if (x >= normal_btn_rect.x && x <= normal_btn_rect.x + normal_btn_rect.w && y >= normal_btn_rect.y && y <= normal_btn_rect.y + normal_btn_rect.h &&is_fullscreen == 1) {
    			SDL_Surface* newScreen = exit_fullscreen(win_w, win_h, win_bpp, &is_fullscreen);
    			if (newScreen) {
        			screen = newScreen;
        			recalculate_button_positions();
    			}
		    }

                    else if (x >= increase_btn_rect.x && x <= increase_btn_rect.x + increase_btn_rect.w &&
                             y >= increase_btn_rect.y && y <= increase_btn_rect.y + increase_btn_rect.h) {
                        int current_volume = Mix_VolumeMusic(-1);
                        if (current_volume < MIX_MAX_VOLUME) {
                            Mix_VolumeMusic(current_volume + 10);
                            if (Mix_PausedMusic()) {
                                Mix_ResumeMusic();
                            }
                        }
                    }
                    else if (x >= decrease_btn_rect.x && x <= decrease_btn_rect.x + decrease_btn_rect.w &&
                             y >= decrease_btn_rect.y && y <= decrease_btn_rect.y + decrease_btn_rect.h) {
                        int current_volume = Mix_VolumeMusic(-1);
                        if (current_volume > 0) {
                            Mix_VolumeMusic(current_volume - 10);
                            if (current_volume - 10 <= 0) {
                                Mix_PauseMusic();
                            }
                        } else {
                            Mix_PauseMusic();
                        }
}
		    else if 
    (x >= x_button_rect.x && x <= x_button_rect.x + x_button_rect.w &&
     y >= x_button_rect.y && y <= x_button_rect.y + x_button_rect.h) {
    main();;
}

			

                    break;

                default:
                    break;
            }
        }

        // Effacer l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Afficher les éléments
	if(is_fullscreen) {
	SDL_BlitSurface(background_f, NULL, screen, NULL); // Fond principal_full
	SDL_BlitSurface(background_m_f, NULL, screen, &destRect_f); // Fond secondaire_full
}
else{
        SDL_BlitSurface(background, NULL, screen, NULL); // Fond principal
        SDL_BlitSurface(background_m, NULL, screen, &destRect); // Fond secondaire
}

        // Afficher les boutons
        SDL_BlitSurface(x_button, NULL, screen, &x_button_rect);
        SDL_BlitSurface(fullscreen_button, NULL, screen, &fullscreen_btn_rect);
        SDL_BlitSurface(normal_button, NULL, screen, &normal_btn_rect);
        SDL_BlitSurface(increase_button, NULL, screen, &increase_btn_rect);
        SDL_BlitSurface(decrease_button, NULL, screen, &decrease_btn_rect);

        // Afficher la barre de volume
        int volume = Mix_VolumeMusic(-1);
        int index = volume * 5 / MIX_MAX_VOLUME;
        if (index > 5) index = 5;
        if (index >= 0 && index < 6) {
            SDL_BlitSurface(volume_images[index], NULL, screen, &volume_rect);
        }

        /*// Afficher le texte "Volume :"
        SDL_Surface *volumeText = TTF_RenderText_Solid(font, "Volume :", textColor);
        if (volumeText) {
            SDL_BlitSurface(volumeText, NULL, screen, &textRect);
            SDL_FreeSurface(volumeText);
        }

        // Afficher le texte "Display Mode :"
        SDL_Surface *displayModeText = TTF_RenderText_Solid(font, "Display Mode :", textColor);
        if (displayModeText) {
            SDL_BlitSurface(displayModeText, NULL, screen, &displayModeTextRect);
            SDL_FreeSurface(displayModeText);
        }*/

        // Mettre à jour l'écran
        SDL_Flip(screen);
    }

    // Libération des ressources
    Mix_FreeMusic(*music);
    SDL_FreeSurface(background);
    SDL_FreeSurface(background_m);
    SDL_FreeSurface(fullscreen_button);
    SDL_FreeSurface(normal_button);
    SDL_FreeSurface(increase_button);
    SDL_FreeSurface(decrease_button);
    for (int i = 0; i < 6; i++) {
        SDL_FreeSurface(volume_images[i]);
    }
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
SDL_Delay(2000);
    return 0;
}
