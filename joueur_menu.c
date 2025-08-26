#include "header.h"




   //declaration des images

        SDL_Surface *background;
	SDL_Surface *joueur1;
        SDL_Surface *screen;
        

	SDL_Surface *mono;
        SDL_Surface *hover_mono;

	SDL_Surface *multi;
        SDL_Surface *hover_multi;

	SDL_Surface *avatar1;
        SDL_Surface *hover_avatar1;

	SDL_Surface *avatar2;
	SDL_Surface *hover_avatar2;

	SDL_Surface *input1;
        SDL_Surface *hover_input1;

	SDL_Surface *input2;
        SDL_Surface *hover_input2;

	SDL_Surface *valider;
        SDL_Surface *hover_valider;


	SDL_Surface *retour;
	SDL_Surface *hover_retour; 
 
   //other declarations
        Mix_Music *music;
        Mix_Chunk *son;
        SDL_Event event;
      
   // declaration des positions des boutons 

	SDL_Rect pos_mono  ={428, 248, 50, 50};
        SDL_Rect pos_multi ={428, 409, 50, 50};
	
	SDL_Rect pos_valider ={1100, 80, 50, 50};
        SDL_Rect pos_retour  ={1100,29 , 51, 51};

	
	SDL_Rect pos_avatar1  ={400, 300, 50, 50};
        SDL_Rect pos_avatar2  ={665, 300, 50, 50};
	
	SDL_Rect pos_input1  ={400, 470, 50, 50};
        SDL_Rect pos_input2  ={665, 470, 50, 50};
	
  
  //menu avatar 




int menu_avatar(int mode) {
 


//chargement image
  chargement_images2( &joueur1, "image_s/joueur.png");
  chargement_images2( &avatar1, "image_s/avatar1.png" );
  chargement_images2( &hover_avatar1, "image_s/havatar1.png" );
  chargement_images2(&avatar2, "image_s/avatar2.png" );
  chargement_images2(&hover_avatar2 , "image_s/havatar2.png" );
  chargement_images2( &input1, "image_s/input1.png");
  chargement_images2( &hover_input1, "image_s/hinput1.png" );
  chargement_images2( &input2, "image_s/input2.png");
  chargement_images2( &hover_input2, "image_s/hinput2.png" );
  chargement_images2(&valider,"image_s/valider.png");
  chargement_images2(&hover_valider,"image_s/hvalider.png");
  chargement_images2(&retour , "image_s/retour.png");
  chargement_images2(&hover_retour,"image_s/hretour.png");
  chargement_images2(&background, "image_s/background.png");


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


  //current buttons version 1 
  SDL_Surface *current_av1 = avatar1;
  SDL_Surface *current_av2 = avatar2;
  SDL_Surface *current_in1 = input1;
  SDL_Surface *current_in2 = input2;
  SDL_Surface *current_retour = retour;
  SDL_Surface *current_valider =valider;

  
  // boucle principale
    int quitter=1;
    int selectav=0, selectin=0;
    int selectav1=0, selectav2=0, selectin1=0, selectin2=0;
     
    int x, y;


  while(quitter){
  while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    break;
               case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE)  main();
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    if (check_hover(pos_avatar1, x, y)) { current_av1 = hover_avatar1; Mix_PlayChannel(-1, son, 0); } else current_av1 = avatar1;
                    if (check_hover(pos_avatar2, x, y)) { current_av2 = hover_avatar2; Mix_PlayChannel(-1, son, 0); } else current_av2 = avatar2;
		    if (check_hover(pos_input1, x, y)) { current_in1 = hover_input1; Mix_PlayChannel(-1, son, 0); } else current_in1 = input1;
                    if (check_hover(pos_input2, x, y)) { current_in2 = hover_input2; Mix_PlayChannel(-1, son, 0); } else current_in2 = input2;
                    if (check_hover(pos_valider, x, y)) { current_valider = hover_valider; Mix_PlayChannel(-1, son, 0); } else current_valider = valider;
                    if (check_hover(pos_retour, x, y)) { current_retour = hover_retour; Mix_PlayChannel(-1, son, 0); } else current_retour = retour;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
		    if (check_hover(pos_input1, x, y)){ selectin1=1; selectin2=0; selectin=1; }
		    if (check_hover(pos_avatar1, x, y)) {selectav1=1; selectav2=0; selectav=1; }
		    if (check_hover(pos_input2, x, y)) {selectin2=1;  selectin1=0; selectin=1;}
		    if (check_hover(pos_avatar2, x, y)){ selectav2=1; selectav1=0; selectav=1; }

                      

                     if (selectin && selectav && mode==1) {
		     if (check_hover(pos_valider, x, y)) joueur_mono(selectav1, selectav2, selectin1, selectin2);
                          }
                      if (selectin && selectav && mode==2) {
		     if (check_hover(pos_valider, x, y)) joueur_multiple(selectav1, selectav2, selectin1, selectin2);
                          }


                    if (check_hover(pos_retour, x, y)) main();
                    break;
            }

       
                      if(selectin1==1) current_in1 = hover_input1; else current_in1 = input1;
                      if(selectin2==1) current_in2 = hover_input2; else current_in2 = input2;
	              if(selectav1==1) current_av1 = hover_avatar1; else current_av1 = avatar1;
	              if(selectav2==1)  current_av2 = hover_avatar2; else current_av2 = avatar2;           	
		    
        }


        SDL_BlitSurface(joueur1, NULL, screen, NULL);
        SDL_BlitSurface(current_av1, NULL, screen, &pos_avatar1);
        SDL_BlitSurface(current_av2, NULL, screen, &pos_avatar2);
	SDL_BlitSurface(current_in1, NULL, screen, &pos_input1);
        SDL_BlitSurface(current_in2, NULL, screen, &pos_input2);
        SDL_BlitSurface(current_valider, NULL, screen, &pos_valider);
        SDL_BlitSurface(current_retour, NULL, screen, &pos_retour);

     

        SDL_Flip(screen);
               





}   


return 0;}


   
int joueur_menu() {

    // vars 
    int quitter=1;
    int x, y; //pos du curseur




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


   // creation de la fenetre
    screen = SDL_SetVideoMode(1204, 800, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Erreur création écran : %s\n", SDL_GetError());
        return 1;
    }



   //chargement des images
  
 
  chargement_images2( &background, "image_s/background.png");
  chargement_images2( &mono, "image_s/mono.png");
  chargement_images2( &hover_mono, "image_s/hmono.png"  );
  chargement_images2( &multi ,"image_s/multi.png"  );
  chargement_images2( &hover_multi,"image_s/hmulti.png"  );
  chargement_images2(&retour , "image_s/retour.png");
  chargement_images2(&hover_retour,"image_s/hretour.png");



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



  //d'abord current buttons vont etre la version normal
  SDL_Surface *current_mono = mono;
  SDL_Surface *current_multi = multi;
  SDL_Surface *current_retour = retour;




 // boucle principale
   
  while(quitter){
  while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    if (check_hover(pos_mono, x, y)) { current_mono = hover_mono; Mix_PlayChannel(-1, son, 0); } else current_mono = mono;
                    if (check_hover(pos_multi, x, y)) { current_multi = hover_multi; Mix_PlayChannel(-1, son, 0); } else current_multi = multi;
                    if (check_hover(pos_retour, x, y)) { current_retour = hover_retour; Mix_PlayChannel(-1, son, 0); } else current_retour = retour;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    if (check_hover(pos_mono, x, y)) menu_avatar(1);
                    if (check_hover(pos_multi, x, y)) menu_avatar(2);
                    if (check_hover(pos_retour, x, y)) main();
                    break;
            }

        }

	SDL_BlitSurface(joueur1, NULL, screen, NULL);
        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_BlitSurface(current_mono, NULL, screen, &pos_mono);
        SDL_BlitSurface(current_multi, NULL, screen, &pos_multi);
        SDL_BlitSurface(current_retour, NULL, screen, &pos_retour);

     

        SDL_Flip(screen);
               





}   




Mix_CloseAudio();
IMG_Quit();
Mix_FreeMusic(music);
Mix_FreeChunk(son);
SDL_Quit();




return 0;}

 

	


