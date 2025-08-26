#include "header.h"

//declaration des images

        SDL_Surface *story1;
        SDL_Surface *story2;
	SDL_Surface *story3;
        SDL_Surface *story4; 
 	SDL_Surface *screen;

	SDL_Event event;

int story_s(){

	chargement_images2( &story1,"img/story1.png");
	chargement_images2(&story2,"img/story2.png");
  	chargement_images2( &story3,"img/story3.png" );
  	chargement_images2(&story4,"img/story4.png" );

    int running = 1;
    int current_page = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                current_page++;
                if (current_page > 4) {
                    main();
                }
            }
        }

        // Show the current story image
        switch (current_page) {
            case 1:
                SDL_BlitSurface(story1, NULL, screen, NULL);
                break;
            case 2:
                SDL_BlitSurface(story2, NULL, screen, NULL);
                break;
            case 3:
                SDL_BlitSurface(story3, NULL, screen, NULL);
                break;
	    case 4:
                SDL_BlitSurface(story4, NULL, screen, NULL);
                break;
        }

        SDL_Flip(screen);
        SDL_Delay(100); // avoid high CPU usage
    }
}






	

