#include "data_struct.h"

/*
 * Hexes are located with cube coordinates.
 * Ressource used: http://www.redblobgames.com/grids/hexagons
 */

//Coordinates data structures
Stor new_stor(int i, int j){
	/* Create new Stor using given coordinates
	 */
	Stor s;
	s.i = i;
	s.j = j;
	return s;
}

Hex new_hex(int r, int q){
	/* Create new Hex using given coordinates
	 */
	Hex h;
	h.r = r;
	h.q = q;
	return h;
}

Cube new_cube(int x, int y, int z){
	/* Create new Cube using given coordinates
	 */
	Cube c;
	c.x = x;
	c.y = y;
	c.z = z;
	return c;
}

//Coordinates conversion
Stor hex_to_stor(int side, Hex h){
	Stor s;
	s.i = h.r + (2*side-2);
	int sjx0;
	if(s.i<side-1 || (s.i>=2*side-1 && s.i<3*side-2)){
		//^: Upper and third star quarters
		sjx0 = linew(side, s.i)-side;
	}else{
		//v: Second and lower star quarters
		sjx0 = side-1;
	}
	s.j = h.q + sjx0;
	return s;
}

Hex stor_to_hex(int side, Stor s){
	Hex h;
	h.r = s.i - (2*side-2);
	int sjx0;
	if(s.i<side-1 || (s.i>=2*side-1 && s.i<3*side-2)){
		//^: Upper and third star quarters
		sjx0 = linew(side, s.i)-side;
	}else{
		//v: Second and lower star quarters
		sjx0 = side-1;
	}
	h.q = s.j - sjx0;
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
	 * default EMPTY everywhere.
	 */

	int total_nb = 6*side*side-4*side+1;

	Content** b = (Content**) malloc(sizeof(Content)*total_nb);
	
	int i;
	for(i=0; i<boardh(side); i++){
		b[i] = (Content*) malloc(sizeof(Content)*linew(side, i));
	}
	return b;
}

int boardh(int side){
	/* Returns board height.
	 */
	return 4*side-3;
}

int linew(int side, int row){
	/* Returns the width of given row in a star of given side length.
	 * row is given in Stor coordinates (s.i).
	 */
	int width = 0;
	if(row<side-1 || (row>=2*side-1 && row<3*side-2)){
		//^: Upper and third star quarters
		width = row+1;
	}else{
		//v: Second and lower star quarters
		width = 4*side-3-row;
	}
	return width;
}

int exists(int side, Hex h){
	/* Returns 1 if coordinates defined in h exist in current map,
	 * otherwise 0.
	 */
	Stor s = hex_to_stor(side, h);
	if(s.i >= 0 && s.i < boardh(side) && s.j >= 0 && s.j < linew(side, s.i)){
		return 1;
	}else{
		return 0;
	}
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

