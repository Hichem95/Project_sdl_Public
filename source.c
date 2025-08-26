
//biblioteques
#include "header.h"




//fonction qui retourne 1 ou 0 en comparant les pos
int check_hover(SDL_Rect button, int x, int y) {
    return (x > button.x && x < button.x + button.w && y > button.y && y < button.y + button.h);
}


// fonction chargement des images

void chargement_images2(SDL_Surface **S, char *path  ){


*S = IMG_Load(path);
   if (!(*S)) {
        printf("Erreur chargement: %s\n", IMG_GetError());
    }


}





