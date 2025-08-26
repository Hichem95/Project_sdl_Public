#include "header.h"
play_menu(){// Déclaration des surfaces
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *background_m = NULL;
SDL_Surface *yes = NULL, *yes_hovered = NULL;
SDL_Surface *no = NULL, *no_hovered = NULL;
SDL_Surface *continuee = NULL, *continuee_hovered = NULL;
SDL_Surface *new = NULL, *new_hovered = NULL;
SDL_Surface *x = NULL;
Mix_Music *music = NULL;
Mix_Chunk *hoverSound = NULL; // Déclaration du son de survol

// Structure pour un bouton
typedef struct {
    SDL_Surface *normal;
    SDL_Surface *hovered;
    SDL_Rect position;
} Button;

// Initialisation des boutons
Button yes_button, no_button, continue_button, new_button;

// Positions des éléments
SDL_Rect destRect = {330,200,800,600};
SDL_Rect x_rect = {1145.5,54.5,51,51};
// Variables de gestion des boutons
int hover_yes = 0, hover_no = 0, hover_continue = 0, hover_new = 0;
int yesClicked = 0;
int displayText = 1; // Contrôle l'affichage du texte

// Fonction pour charger une image avec gestion d'erreur
SDL_Surface *loadImage(const char *filename) {
    SDL_Surface *loadedImage = IMG_Load(filename);
    if (!loadedImage) {
        printf("Erreur chargement image %s : %s\n", filename, IMG_GetError());
    }
    return loadedImage;
}

// Fonction pour redimensionner une surface
SDL_Surface *resizeSurface(SDL_Surface *surface, int newWidth, int newHeight) {
    SDL_Surface *resizedSurface = SDL_CreateRGBSurface(
        SDL_SWSURFACE, newWidth, newHeight, 32,
        surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    if (!resizedSurface) {
        printf("Erreur création surface redimensionnée : %s\n", SDL_GetError());
        return NULL;
    }

    // Redimensionnement avec SDL_SoftStretch
    SDL_SoftStretch(surface, NULL, resizedSurface, NULL);
    return resizedSurface;
}

// Fonction pour initialiser un bouton
void initButton(Button *button, const char *normalImg, const char *hoveredImg, int x, int y, int width, int height) {
    // Charger les images
    SDL_Surface *normal = loadImage(normalImg);
    SDL_Surface *hovered = loadImage(hoveredImg);

    // Redimensionner les images
    button->normal = resizeSurface(normal, width, height);
    button->hovered = resizeSurface(hovered, width, height);

    // Libérer les surfaces originales
    SDL_FreeSurface(normal);
    SDL_FreeSurface(hovered);

    // Définir la position du bouton
    button->position.x = x;
    button->position.y = y;

    printf("Taille de l'image redimensionnée: largeur = %d, hauteur = %d\n", button->normal->w, button->normal->h);
}

// Fonction pour vérifier si la souris est sur un bouton
int isMouseOver(Button *button, int mouseX, int mouseY) {
    return (mouseX >= button->position.x && mouseX <= button->position.x + button->normal->w &&
            mouseY >= button->position.y && mouseY <= button->position.y + button->normal->h);
}


// Fonction pour afficher un bouton (normal ou hover)
void renderButton(Button *button, int isHovered) {
    SDL_Surface *image;
if (isHovered) {
    image = button->hovered;  // Si la souris est sur le bouton, on affiche l’image "hovered"
} else {
    image = button->normal;    // Sinon, on affiche l’image "normale"
}
    SDL_BlitSurface(image, NULL, screen, &button->position);
}

 // Paramètres inutilisés
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }

    // Chargement de la police
    TTF_Font *font = TTF_OpenFont("font/ARCADECLASSIC.TTF", 44);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

       SDL_Color textColor = {84, 74, 60,}; 

    // Création de la fenêtre
    screen = SDL_SetVideoMode(1204, 800, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Erreur création écran : %s\n", SDL_GetError());
        return 1;
    }

    // Chargement des images
    background = loadImage("img/bg1.png");
    background_m = loadImage("img/bg2.png");
    x = loadImage("img/x.png");

    // Initialisation des boutons avec redimensionnement
    initButton(&yes_button, "img/yes.png", "img/yes_hovered.png", 521, 456, 94, 94); // Redimensionné à 200x80
    initButton(&no_button, "img/no.png", "img/no_hovered.png", 684, 456, 92, 92);   // Redimensionné à 200x80
    initButton(&continue_button, "img/continue.png", "img/continue_hovered.png", 602, 475.5, 389, 139); // Redimensionné à 300x150
    initButton(&new_button, "img/new.png", "img/new_hovered.png", 602, 324.5, 389, 139); // Redimensionné à 300x140


    // Calculer les positions centrées
    int screenWidth = 1204;  // Largeur de l'écran
    int screenHeight = 800;  // Hauteur de l'écran

    // Centrer les boutons continue et new sur le fond 'background_m'
    continue_button.position.x = (screenWidth - continue_button.normal->w + 50) / 2;
    continue_button.position.y = (screenHeight - continue_button.normal->h + 100) / 3;  // Ajustez pour le bon alignement

    new_button.position.x = (screenWidth - new_button.normal->w + 50) / 2;
    new_button.position.y = (screenHeight - new_button.normal->h + 300) / 2;  // Ajustez pour un espacement adéquat

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        return 1;
    }

    // Chargement du son de survol
    hoverSound = Mix_LoadWAV("son/son.wav");
    if (!hoverSound) {
        printf("Erreur chargement son : %s\n", Mix_GetError());
        return 1;
    }

    // Chargement et lecture de la musique
    music = Mix_LoadMUS("son/music.mp3");
    if (!music) {
        printf("Erreur chargement musique : %s\n", Mix_GetError());
        return 1;
    }
    Mix_PlayMusic(music, -1);

    // Boucle principale
    int running = 1;
    SDL_Event event;
   int cc = 0;
   int retour=0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
		
                case SDL_MOUSEMOTION:
                    hover_yes = isMouseOver(&yes_button, event.motion.x, event.motion.y);
                    hover_no = isMouseOver(&no_button, event.motion.x, event.motion.y);
                    hover_continue = isMouseOver(&continue_button, event.motion.x, event.motion.y);
                    hover_new = isMouseOver(&new_button, event.motion.x, event.motion.y);
                    // Jouer le son lors du survol
                    if (hover_yes || hover_no) {
                        Mix_PlayChannel(-1, hoverSound, 0); // Jouer le son de survol
                    }
		    if(cc==1 && hover_continue || hover_new) {
                        Mix_PlayChannel(-1, hoverSound, 0); // Jouer le son de survol
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (hover_yes) {
                            yesClicked = 1;
                            displayText = 0; // Masquer le texte lorsque "Yes" est cliqué
			    cc=1;
                        }
                        if (hover_continue && cc==1) {
                            printf("continuer...\n");
                            displayText = 0;
                        }
                        if (hover_new && cc==1) {
                            displayText = 1;
                            joueur_menu();
                            displayText = 0;
			    //khedmt souhaaaaaaaaa
			  
                        }
			//retour ll menu principal::
			if (event.button.x >= x_rect.x && event.button.x <= x_rect.x + x_rect.w &&event.button.y >= x_rect.y && event.button.y <= x_rect.y + x_rect.h) {
    				main(); 
			}

			
                    }	
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_n && cc==1) {
                        displayText = 1;
                        printf("Nouveau...\n");
                        joueur_menu();
			//khedmt souhaaaaaaaaaa
			
                    }
		if (event.key.keysym.sym == SDLK_ESCAPE) {
		    running = 0;  // quitter
		}
		if (event.key.keysym.sym == SDLK_c && cc==1) {
                        displayText = 1;
                        printf("continuer...\n");
                        displayText = 0;}
		if (event.key.keysym.sym == SDLK_y) {
                     yesClicked = 1;
                     displayText = 0; // Masquer le texte lorsque "Yes" est cliqué
		     cc=1;
		}
		
                    break;  
         
		
}
        }

        // Effacer l'écran (tout le fond) avant de redessiner
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Afficher les éléments de fond
        SDL_BlitSurface(background, NULL, screen, NULL);
        if(!yesClicked)
		SDL_BlitSurface(background_m, NULL, screen, &destRect);
        SDL_BlitSurface(x, NULL, screen, &x_rect);

        // Afficher le texte si nécessaire
        if (displayText) {
            SDL_Surface *displayModeText = TTF_RenderText_Solid(font, "would you like to", textColor);
	    SDL_Surface *displayModeText1 = TTF_RenderText_Solid(font, " save your progress ?", textColor);
            if (displayModeText) {
                SDL_Rect textRect = {450, 300,0,0}; // Position ajustée pour centrer le texte
		SDL_Rect textRect1 = {430, 350,0,0}; // Position ajustée pour centrer le texte
                SDL_BlitSurface(displayModeText, NULL, screen, &textRect);
		SDL_BlitSurface(displayModeText1, NULL, screen, &textRect1);
                SDL_FreeSurface(displayModeText); // Libérez la surface après utilisation
		SDL_FreeSurface(displayModeText1); // Libérez la surface après utilisation
            }
        }
	
        // Afficher les boutons avec les bonnes variables
        if (!yesClicked && !cc) {
            renderButton(&yes_button, hover_yes);
            renderButton(&no_button, hover_no);
        } else {
            renderButton(&continue_button, hover_continue);
            renderButton(&new_button, hover_new);
        }

        SDL_Flip(screen); // Mettre à jour l'écran
    }

    // Libération des ressources
    SDL_FreeSurface(background);
    SDL_FreeSurface(background_m);
    SDL_FreeSurface(x);
    SDL_FreeSurface(yes_button.normal);
    SDL_FreeSurface(yes_button.hovered);
    SDL_FreeSurface(no_button.normal);
    SDL_FreeSurface(no_button.hovered);
    SDL_FreeSurface(continue_button.normal);
    SDL_FreeSurface(continue_button.hovered);
    SDL_FreeSurface(new_button.normal);
    SDL_FreeSurface(new_button.hovered);
    Mix_FreeMusic(music);
    Mix_FreeChunk(hoverSound); // Libérer le son
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

