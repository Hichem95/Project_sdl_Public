prog:main.o source.o play.o joueur_menu.o joueur_mono.o joueur_multiple.o joueur_source.o enemy.o animation_background.o collision_parfaite.o collision_bb.o minimap.o meilleur.o background.o option.o pause.o quiz.o enigme.o story.o
	gcc main.o source.o play.o joueur_menu.o joueur_mono.o joueur_multiple.o joueur_source.o enemy.o animation_background.o collision_parfaite.o collision_bb.o minimap.o meilleur.o background.o option.o pause.o quiz.o enigme.o story.o -o prog -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lm -g

option.o:option.c header.h 
	gcc -c option.c -g
story.o:story.c header.h 
	gcc -c story.c -g
enigme.o:enigme.c enigme.h header.h
	gcc -c enigme.c -g
quiz.o:quiz.c enigme.h header.h 
	gcc -c quiz.c -g
pause.o:pause.c header.h 
	gcc -c pause.c -g
background.o: background.c background.h header.h
	gcc -c background.c -o background.o
meilleur.o: meilleur.c meilleur.h header.h
	gcc -c meilleur.c -o meilleur.o

minimap.o: minimap.c minimap.h header.h
	gcc -c minimap.c -o minimap.o

collision_bb.o: collision_bb.c collision_bb.h animation_background.h header.h
	gcc -c collision_bb.c -o collision_bb.o

collision_parfaite.o: collision_parfaite.c collision_parfaite.h animation_background.h header.h
	gcc -c collision_parfaite.c -o collision_parfaite.o

animation_background.o: animation_background.c animation_background.h header.h
	gcc -c animation_background.c -o animation_background.o

joueur_source.o:joueur_source.c  header.h
	gcc -c joueur_source.c -g

enemy.o:enemy.c  header.h
	gcc -c enemy.c -g

joueur_multiple.o:joueur_multiple.c header.h background.h
	gcc -c joueur_multiple.c -g

joueur_mono.o:joueur_mono.c header.h background.h
	gcc -c joueur_mono.c -g

joueur_menu.o:joueur_menu.c header.h
	gcc -c joueur_menu.c -g

play.o:play.c header.h 
	gcc -c play.c -g

source.o:source.c header.h 
	gcc -c source.c -g

main.o:main.c header.h 
	gcc -c main.c -g

clean:
	rm -f *.o prog

