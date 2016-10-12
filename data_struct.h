#ifndef DATA_STRUCT_H_INCLUDED
#define DATA_STRUCT_H_INCLUDED

/* Data structures
 * How are coordinates, board, cells stored, how they relate,
 * is defined herein.
 * Hexes are located with cube coordinates.
 * Ressource used: http://www.redblobgames.com/grids/hexagons
 */

#include <stdlib.h>

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

//Enums
typedef enum content { EMPTY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, INVALID } Content;
typedef enum zone { CENTER, TOP, TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT, BOT, NOWHERE } Zone;

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
Zone get_zone(int, Hex);

//Cell functions
Content get_ct(Content**, int, Hex);
void set_ct(Content**, int, Hex, Content);
Hex* neighbors(int, Hex, int*);
#endif

