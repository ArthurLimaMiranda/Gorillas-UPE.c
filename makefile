all: main

main: main.c
	gcc main.c -lncurses -lm
