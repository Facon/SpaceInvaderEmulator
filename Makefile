CC=g++
DFLAGS=-c -ggdb -Wall
CFLAGS=-c -O2 -Wall
FLAGS=$(DFLAGS)
LIBS=-lSDL
OBJ=main.o

invaders: main.cpp
	$(CC) $(FLAGS) main.cpp 8080.cpp utils.cpp invaders.cpp
	$(CC) main.o 8080.o utils.o invaders.o $(LIBS) -o invaders

clean:
	rm -f -r *~
	rm -f -r *.o
	rm -f -r *.obj
	
clean_all:
	rm -f -r *~
	rm -f -r *.o
	rm -f -r *.obj
	rm -f -r *~.cpp
	rm -f -r *.~cpp
	rm -f -r *~.h
	rm -f -r invaders