prog: suiv.o main.o
	gcc suiv.o main.o -o prog -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

joueur.o: suiv.c suiv.h
	gcc -c suiv.c

main.o: main.c suiv.h
	gcc -c main.c

clean:
	rm -f *.o prog

