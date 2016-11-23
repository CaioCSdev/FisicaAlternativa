CC=gcc
CFLAGS=-Wall -pedantic 
LIBS=-lm

DEPS = src/input.h src/prints.h src/color.h
SRC = src/main.c src/input.c src/prints.c src/color.c
altPhisics: $(DEPS) $(SRC)
	gcc -o bin/altPhisics $(SRC) $(DEPS) $(CFLAGS) $(LIBS)
