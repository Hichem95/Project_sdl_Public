//biblioteques
#include "header.h"
#include "meilleur.h"

// déclaration des variables

SDL_Surface *logo_image = NULL;
SDL_Surface *logo_image2 = NULL;


SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;

SDL_Surface *button_play = NULL;
SDL_Surface *button_play_v2 = NULL;

SDL_Surface *button_option = NULL;
SDL_Surface *button_option_v2 = NULL;


SDL_Surface *button_high = NULL;
SDL_Surface *button_high_v2 = NULL;

SDL_Surface *button_story = NULL;
SDL_Surface *button_story_v2 = NULL;

SDL_Surface *button_quit = NULL;
SDL_Surface *button_quit_v2 = NULL;

SDL_Surface *game_name; 

Mix_Chunk *son = NULL;
Mix_Music *music = NULL;

TTF_Font *font;
SDL_Color textColor = {247, 185, 88};

SDL_Event event;

//pos bouttons
SDL_Rect play={483,173,50,50};
SDL_Rect option={483,268,50,50};
SDL_Rect high={417,370,50,50};
SDL_Rect story={483,467,50,50};
SDL_Rect quit={483,569,50,50};

SDL_Rect logo={40,34, 50, 50 };
/*SDL_Rect text= {290, 90, 50, 50}; */






int main(int argc, char *argv[]) {

    //vars 
    int quitter=1;
    int x, y; //pos du curseur
    joueur j;







    // initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    // initialisation de SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        return 1;
    }


    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }

    // Chargement de la police 
    font = TTF_OpenFont("alagard.ttf", 50);  
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

   
   // creation de la fenetre
    screen = SDL_SetVideoMode(1204, 800   , 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Erreur création écran : %s\n", SDL_GetError());
        return 1;
    }


  //chargement des images
  
  chargement_images2( &logo_image, "images_/logo.png");
  chargement_images2( &background, "images_/bg.png");
  chargement_images2( &button_play, "images_/boutton_play.png"  );
  chargement_images2( &button_play_v2 ,"images_/boutton_play_v2.png"  );
  chargement_images2( &button_option,"images_/boutton_option.png"  );
  chargement_images2( &button_option_v2, "images_/boutton_option_v2.png" );
  chargement_images2( &button_high, "images_/boutton_high.png" );
  chargement_images2(&button_high_v2, "images_/boutton_high_v2.png" );
  chargement_images2(&button_story , "images_/boutton_story.png" );
  chargement_images2(&button_story_v2 , "images_/boutton_story_v2.png");
  chargement_images2(&button_quit,"images_/boutton_quit.png");
  chargement_images2(&button_quit_v2,"images_/boutton_quit_v2.png");



  // Afficher le texte 
       /* game_name = TTF_RenderText_Solid(font, "SHATTERED OATH", textColor);
        if (!game_name) {
        printf("Erreur chargement texe : %s\n", Mix_GetError());
        return 1;    
        }*/

// initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError()); 
    return 1;
    }

    // Chargement de la musique
    music = Mix_LoadMUS("son/music.mp3");
    if (!music) {
        printf("Erreur chargement musique : %s\n", Mix_GetError());
        return 1;
    }
    
    // chargement du son bref
   son = Mix_LoadWAV("son/son.wav");
   if (!son) {
        printf("Erreur chargement son bref : %s\n", Mix_GetError());
        return 1;}


   // jouer en boucle la musique
   Mix_PlayMusic(music, -1); 
    


  //d'abord current buttons vont etre la v1
  SDL_Surface *current_play = button_play;
  SDL_Surface *current_option = button_option;
  SDL_Surface *current_high = button_high;
  SDL_Surface *current_story = button_story;
  SDL_Surface *current_quit = button_quit;

 //variables to track the hover
int play_hovered = 0;
int option_hovered = 0;
int high_hovered = 0;
int story_hovered = 0;
int quit_hovered = 0;

   

  // boucle principale
   
  while(quitter){
  while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_p) play_menu();
                    if (event.key.keysym.sym == SDLK_o) printf("page high scores yahya");
                    if (event.key.keysym.sym == SDLK_h) printf("page high scores yahya\n");
                    if (event.key.keysym.sym == SDLK_s) printf("page Story someone");
                    if (event.key.keysym.sym == SDLK_ESCAPE) quitter = 0;
                    break;
               case SDL_MOUSEMOTION:
               x = event.motion.x;
               y = event.motion.y;

                  if (check_hover(play, x, y)) {current_play = button_play_v2; if (!play_hovered) { Mix_PlayChannel(-1, son, 0); play_hovered = 1; } } 
                  else { current_play = button_play; play_hovered = 0;}

    
                  if (check_hover(option, x, y)) {current_option = button_option_v2; if (!option_hovered) {Mix_PlayChannel(-1, son, 0); option_hovered = 1;} } 
                  else {current_option = button_option;option_hovered = 0;}

                  if (check_hover(high, x, y)) {current_high = button_high_v2; if (!high_hovered) {Mix_PlayChannel(-1, son, 0); high_hovered = 1;} } 
                  else {current_high = button_high; high_hovered = 0;}

                  if (check_hover(story, x, y)) {current_story = button_story_v2; if (!story_hovered) {Mix_PlayChannel(-1, son, 0); story_hovered = 1;} } 
                  else {current_story = button_story; story_hovered = 0;}

                  if (check_hover(quit, x, y)) {current_quit = button_quit_v2; if (!quit_hovered) {Mix_PlayChannel(-1, son, 0); quit_hovered = 1;} } 
                  else {current_quit = button_quit; quit_hovered = 0;}
                  
                  break;

                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    if (check_hover(play, x, y)) play_menu();
                    if (check_hover(option, x, y)) option_menu(&music) ;
                    if (check_hover(high, x, y)) meilleur(j);
                    if (check_hover(story, x, y)) story_s();
                    if (check_hover(quit, x, y)) quitter = 0;
                    break;
            }

        }


        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_BlitSurface(current_play, NULL, screen, &play);
        SDL_BlitSurface(current_option, NULL, screen, &option);
        SDL_BlitSurface(current_high, NULL, screen, &high);
        SDL_BlitSurface(current_story, NULL, screen, &story);
        SDL_BlitSurface(current_quit, NULL, screen, &quit);
        SDL_BlitSurface(logo_image, NULL, screen, &logo);
        /*SDL_BlitSurface(game_name, NULL, screen, &text);*/

     

        SDL_Flip(screen);
               





}   







//free
    SDL_FreeSurface(logo_image);
    SDL_FreeSurface(logo_image2);
    SDL_FreeSurface(background);
    SDL_FreeSurface(button_play);
    SDL_FreeSurface(button_play_v2);
    SDL_FreeSurface(button_option);
    SDL_FreeSurface(button_option_v2);
    SDL_FreeSurface(button_high);
    SDL_FreeSurface(button_high_v2);
    SDL_FreeSurface(button_story);
    SDL_FreeSurface(button_story_v2);
    SDL_FreeSurface(button_quit);
    SDL_FreeSurface(button_quit_v2);
    SDL_FreeSurface(game_name);
//quit stuff
Mix_CloseAudio();
IMG_Quit();
TTF_CloseFont(font);
TTF_Quit();
Mix_FreeMusic(music);
Mix_FreeChunk(son);
SDL_Quit();
return 0; }




