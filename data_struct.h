#ifndef DATA_STRUCT_H_INCLUDED
#define DATA_STRUCT_H_INCLUDED
#include <stdlib.h>
#include <ncurses.h>

//Coordinates data structures
typedef struct stor{
	//Storage coordinates
	int i;
	int j;
} Stor;

typedef struct hex{
	//Axial (or Hex) coordinates
	int r;
	int q;
} Hex;

typedef struct cube{
	//Cube coordinates
	int x;
	int y;
	int z;
} Cube;

typedef enum content { EMPTY, RED, GREEN, BLUE, YELLOW, WHITE, PURPLE, FORBIDDEN } Content;

//Coordinates conversion
Stor hex_to_stor(int, Hex);
Hex stor_to_hex(int, Stor);
Cube hex_to_cube(Hex);
Hex cube_to_hex(Cube);
int distance(Hex, Hex);

//Board functions
Content** init_board(int);
int boardh(int);
int linew(int, int);
Content get_ct(Content**, int, Hex);
void set_ct(Content**, int, Hex, Content);
#endif

