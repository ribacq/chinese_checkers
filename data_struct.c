#include "data_struct.h"

/*
 * Hexes are located with cube coordinates.
 * Ressource used: http://www.redblobgames.com/grids/hexagons
 */

//Coordinates conversion
Stor hex_to_stor(int side, Hex h){
	Stor s;
	s.i = h.r + (side-1);
	s.j = h.q + (side-1) + (h.r<0 ? h.r : 0);
	return s;
}

Hex stor_to_hex(int side, Stor s){
	Hex h;
	h.r = s.i - (side-1);
	h.q = s.j - (side-1) - (h.r<0 ? h.r : 0);
	return h;
}

Cube hex_to_cube(Hex h){
	Cube c;
	c.z = h.r;
	c.x = h.q;
	c.y = 0 - c.x - c.z;
	return c;
}

Hex cube_to_hex(Cube c){
	Hex h;
	h.r = c.z;
	h.q = c.x;
	return h;
}

int distance(Hex h1, Hex h2){
	Cube c1 = hex_to_cube(h1);
	Cube c2 = hex_to_cube(h2);
	return (abs(c1.x-c2.x)+abs(c1.y-c2.y)+abs(c1.z-c2.z))/2;
}

//Board
Content** init_board(int side){
	/* Contents are stored in a two-dimension table initialized with by
	 * default EMPTY everywhere, in order for the board to be an hexagon.
	 */

	int total_nb = 3*side*side-3*side+1;
	int diagonal = 2*side-1;

	Content** b = (Content**) malloc(sizeof(Content)*total_nb);
	
	int i;
	for(i=0; i<diagonal; i++){
		b[i] = (Content*) malloc(sizeof(Content)*linew(side, i));
	}
	return b;
}

int linew(int side, int row){
	/* Returns the width of given row in a hexagon of given side length.
	 */
	return 2*side-1-abs(side-row-1);
}

Content get_ct(Content** b, int side, Hex h){
	/* Returns the content of given hex
	 */
	Stor s = hex_to_stor(side, h);
	return b[s.i][s.j];
}

void set_ct(Content** b, int side, Hex h, Content c){
	/* Sets the content of given Hex
	 */
	Stor s = hex_to_stor(side, h);
	b[s.i][s.j] = c;
	return;
}

