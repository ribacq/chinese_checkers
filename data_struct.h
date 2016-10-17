#ifndef DATA_STRUCT_H_INCLUDED
#define DATA_STRUCT_H_INCLUDED

/**
 * \file data_struct.h
 * \brief Board and cells data structures
 *
 * How are coordinates, board, cells stored, how they relate,
 * is defined herein.
 *
 * Hexes are located with cube coordinates.
 * Ressource used: http://www.redblobgames.com/grids/hexagons
 */

#include <stdlib.h>

//Coordinates data structures

///\brief Storage coordinates: indexes in Content[][]
typedef struct{
	int i; ///< \brief line, top-most having i=0.
	int j; ///< \brief position on the line, left-most having j=0
} Stor;
///\brief Constructor for Stor.
Stor new_stor(int, int);

///\brief Axial (or Hex) coordinates: most-used coordinates system
typedef struct{
	int r; ///< \brief row, center-most having r=0.
	int q; ///< \brief column, center-most having q=0.
} Hex;
///\brief Constructor for Hex.
Hex new_hex(int, int);

///\brief Cubic coordinates: using the hex-cube analogy, x+y+z = 0.
typedef struct{
	int x; ///< \brief bottom-left to top-right
	int y; ///< \brief bottom-right to top-left
	int z; ///< \brief top to bottom
} Cube;
///\brief Constructor for Cube.
Cube new_cube(int, int, int);

//Enums
///\brief Cell content: empty, 6 colors, and an error value.
typedef enum content { EMPTY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, INVALID } Content;
///\brief Board zones: center, 6 corners, and an arror value.
typedef enum zone { CENTER, TOP, TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT, BOT, NOWHERE } Zone;

///\defgroup coord_conv_gp Coordinates conversion functions
///\{
Stor hex_to_stor(const int, Hex);
Hex stor_to_hex(const int, Stor);
Cube hex_to_cube(Hex);
Hex cube_to_hex(Cube);
///\}

///\defgroup board_fn_gp Board functions
///\{
Content** init_board(const int);
int boardh(const int);
int linew(const int, int);
Zone get_zone(const int, Hex);
Zone get_opposite(Zone);
int get_corner_size(const int);
Hex* get_corner_array(const int, Zone);
void set_corner_ct(Content**, const int, Zone, Content);
///\}

///\defgroup cells_fn_gp Cells functions
///\{
Content get_ct(Content**, const int, Hex);
void set_ct(Content**, const int, Hex, Content);
int distance(Hex, Hex);
Hex* neighbors(const int, Hex, int*);
int in_cell_array(Hex, int, Hex*);
///\}
#endif

