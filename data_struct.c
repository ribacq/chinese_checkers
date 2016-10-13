#include "data_struct.h"

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
Stor hex_to_stor(const int side, Hex h){
	Stor s;
	s.i = h.r + (2*side-2);
	int sjq0;
	if(s.i<side-1 || (s.i>=2*side-1 && s.i<3*side-2)){
		//^: Upper and third star quarters
		sjq0 = linew(side, s.i)-side;
	}else{
		//v: Second and lower star quarters
		sjq0 = side-1;
	}
	s.j = h.q + sjq0;
	return s;
}

Hex stor_to_hex(const int side, Stor s){
	Hex h;
	h.r = s.i - (2*side-2);
	int sjq0;
	if(s.i<side-1 || (s.i>=2*side-1 && s.i<3*side-2)){
		//^: Upper and third star quarters
		sjq0 = linew(side, s.i)-side;
	}else{
		//v: Second and lower star quarters
		sjq0 = side-1;
	}
	h.q = s.j - sjq0;
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
Content** init_board(const int side){
	/* Contents are stored in a two-dimension table initialized with by
	 * default EMPTY everywhere and colors in the corners.
	 */

	//General creation
	int total_nb = 6*side*side-4*side+1;
	Content** b = (Content**) malloc(sizeof(Content)*total_nb);
	
	//Line per line
	int i, j;
	for(i=0; i<boardh(side); i++){
		b[i] = (Content*) malloc(sizeof(Content)*linew(side, i));
		for(j=0; j<linew(side, i); j++){
			switch(get_zone(side, stor_to_hex(side, new_stor(i, j)))){
			case TOP:
				b[i][j] = RED;
				break;

			case TOP_LEFT:
				b[i][j] = GREEN;
				break;

			case BOT_LEFT:
				b[i][j] = YELLOW;
				break;

			case BOT:
				b[i][j] = BLUE;
				break;

			case BOT_RIGHT:
				b[i][j] = MAGENTA;
				break;

			case TOP_RIGHT:
				b[i][j] = CYAN;
				break;

			default:
				b[i][j] = EMPTY;
				break;
			}
		}
	}

	return b;
}

int boardh(const int side){
	/* Returns board height.
	 */
	return 4*side-3;
}

int linew(const int side, int row){
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

Zone get_zone(const int side, Hex h){
	/* Returns the zone wherein given hex is located.
	 */
	Stor s = hex_to_stor(side, h);
	Cube c = hex_to_cube(h);
	if(s.i < 0 || s.i >= boardh(side) || s.j < 0 || s.j >= linew(side, s.i)){
		return NOWHERE;
	}else if(c.z < 1-side){
		return TOP;
	}else if(c.z > side-1){
		return BOT;
	}else if(c.x > side-1){
		return TOP_RIGHT;
	}else if(c.x < 1-side){
		return BOT_LEFT;
	}else if(c.y > side-1){
		return TOP_LEFT;
	}else if(c.y < 1-side){
		return BOT_RIGHT;
	}else{
		return CENTER;
	}
}

//Cell functions
Content get_ct(Content** b, const int side, Hex h){
	/* Returns the content of given hex
	 */
	if(get_zone(side, h) == NOWHERE) return INVALID;
	Stor s = hex_to_stor(side, h);
	return b[s.i][s.j];
}

void set_ct(Content** b, const int side, Hex h, Content c){
	/* Sets the content of given Hex
	 */
	Stor s = hex_to_stor(side, h);
	b[s.i][s.j] = c;
	return;
}

Hex* neighbors(const int side, Hex h, int* len){
	/* Returns an array of h’s neighboring cells
	 * Sets len to the neighbors’ number
	 */
	int i = 0;
	Hex tmp_h;
	Hex* neighs = (Hex*) malloc(6*sizeof(Hex));

	//Level-right
	tmp_h = new_hex(h.r, h.q+1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;

	//Up-right
	tmp_h = new_hex(h.r-1, h.q+1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;

	//Up-left
	tmp_h = new_hex(h.r-1, h.q);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;

	//Level-left
	tmp_h = new_hex(h.r, h.q-1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;

	//Down-left
	tmp_h = new_hex(h.r+1, h.q-1);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;

	//Down-right
	tmp_h = new_hex(h.r+1, h.q);
	if(get_zone(side, tmp_h) != NOWHERE) neighs[i++] = tmp_h;

	*len = i;
	neighs = realloc(neighs, sizeof(Hex)*i);
	return neighs;
}

