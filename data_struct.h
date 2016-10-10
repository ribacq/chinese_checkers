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
Stor new_stor(int, int);

typedef struct hex{
	//Axial (or Hex) coordinates
	int r;
	int q;
} Hex;
Hex new_hex(int, int);

typedef struct cube{
	//Cube coordinates
	int x;
	int y;
	int z;
} Cube;
Cube new_cube(int, int, int);

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
int exists(int, Hex);
Content get_ct(Content**, int, Hex);
void set_ct(Content**, int, Hex, Content);
#endif

