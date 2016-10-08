#ifndef DATA_STRUCT_H_INCLUDED
#define DATA_STRUCT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

//Coordinates data structures
typedef struct hex{
	int r;
	int q;
} Hex;

typedef struct cube{
	int x;
	int y;
	int z;
} Cube;

typedef enum content { EMPTY, RED, GREEN, BLUE, YELLOW, WHITE, PURPLE, FORBIDDEN } Content;

//Coordinates conversion
Cube hex_to_cube(Hex);
Hex cube_to_hex(Cube);

//Board functions
Content* init_board(int);
void test_print_board(Content*, int);
void print_board(Content*, int);

#endif

