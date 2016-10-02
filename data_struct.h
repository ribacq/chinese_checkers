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
const char DISP_CHARS[8] = {'o', 'r', 'g', 'b', 'y', 'w', 'p', ' '};

//Coordinates conversion
Cube hex_to_cube(Hex);
Hex cube_to_hex(Cube);

//Board functions
Content* init_board(int);
void test_print_board(Content*, int);
void print_board(Content*, int);
