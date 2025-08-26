#include "header.h"


//declaration des images

        SDL_Surface *imgLevelComplete;
        SDL_Surface *imgGameOver; 
 	SDL_Surface *screen;
        

	SDL_Surface *next;
        SDL_Surface *hover_next;

	SDL_Surface *menu;
        SDL_Surface *hover_menu;

	SDL_Surface *replay;
        SDL_Surface *hover_replay;

	//declaration des positions du bontons:
	
	SDL_Rect pos_menu  ={620, 470, 50, 50};
        SDL_Rect pos_next ={370, 470, 50, 50};
	SDL_Rect pos_replay  ={370, 470, 50, 50};

	SDL_Event event;
	int level;



//menu:

	int levelcomplete(){

	chargement_images2( &imgLevelComplete, "img/youwon.png");
	chargement_images2(&next, "img/next.png");
  	chargement_images2( &hover_next, "img/hnext.png" );
  	chargement_images2(&menu, "img/menu.png" );
  	chargement_images2(&hover_menu , "img/hmenu.png" );




	SDL_Surface *current_next = next;
 	SDL_Surface *current_menu = menu;
	
	int quitter=1;
	int x,y;
    	int selectav1=0, selectav2=0, selectin1=0, selectin2=0;




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
                    	if (check_hover(pos_menu, x, y)) { current_menu = hover_menu;  } else current_menu = menu;
                    	if (check_hover(pos_next, x, y)) { current_next = hover_next;  } else current_next = next;
                    	break;
                	case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    if (check_hover(pos_menu, x, y)) main();
                    if (check_hover(pos_next, x, y)) quiz();
                    break;
            }

        }

        SDL_BlitSurface(imgLevelComplete, NULL, screen, NULL);
        SDL_BlitSurface(current_menu, NULL, screen, &pos_menu);
        SDL_BlitSurface(current_next, NULL, screen, &pos_next);

     

        SDL_Flip(screen);
               

}   




 

return 0;}


//menu:

	int gameover(){

  	chargement_images2( &imgGameOver, "img/youlost.png");
  	chargement_images2(&menu, "img/menu.png" );
  	chargement_images2(&hover_menu , "img/hmenu.png" );
        chargement_images2(&replay, "img/replay.png" );
  	chargement_images2(&hover_replay , "img/hreplay.png" );



 	SDL_Surface *current_menu = menu;
  	SDL_Surface *current_replay = replay;

	
	int quitter=1;
	int x,y;




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
                    	if (check_hover(pos_menu, x, y)) { current_menu = hover_menu;} else current_menu = menu;
                    	if (check_hover(pos_replay, x, y)) { current_replay = hover_replay;} else current_replay= replay;
                    	break;
                	case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    if (check_hover(pos_menu, x, y)) main();
                    if (check_hover(pos_replay, x, y)) joueur_menu();
                    break;
            }

        }

	SDL_BlitSurface(imgGameOver, NULL, screen, NULL);
        SDL_BlitSurface(current_menu, NULL, screen, &pos_menu);
        SDL_BlitSurface(current_replay, NULL, screen, &pos_replay);

     

        SDL_Flip(screen);
               

}   



 

return 0;}
	
  	 




