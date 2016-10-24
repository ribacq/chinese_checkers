#include "data_struct.h"

//Coordinates data structures
Stor new_stor(int i, int j){
	Stor s;
	s.i = i;
	s.j = j;
	return s;
}

Hex new_hex(int r, int q){
	Hex h;
	h.r = r;
	h.q = q;
	return h;
}

Cube new_cube(int x, int y, int z){
	Cube c;
	c.x = x;
	c.y = y;
	c.z = z;
	return c;
}

//Coordinates conversion
///\brief Hex to Stor conversion. Needs side.
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

///\brief Stor to Hex conversion. Needs side.
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

///\brief Hex to Cube conversion
Cube hex_to_cube(Hex h){
	Cube c;
	c.z = h.r;
	c.x = h.q;
	c.y = 0 - c.x - c.z;
	return c;
}

///\brief Cube to Hex conversion
Hex cube_to_hex(Cube c){
	Hex h;
	h.r = c.z;
	h.q = c.x;
	return h;
}

//Board
/**
 * \brief Returns the default staring board.
 * 
 * Contents are stored in a two-dimensional table initialized with by defalut
 * EMPTY everywhere and INVALID in the corners.
 */
Content** init_board(const int side){
	//General creation
	int total_nb = 6*side*side-4*side+1;
	Content** b = (Content**) malloc(sizeof(Content)*total_nb);
	
	//Line per line
	int i, j;
	for(i=0; i<boardh(side); i++){
		b[i] = (Content*) malloc(sizeof(Content)*linew(side, i));
		for(j=0; j<linew(side, i); j++){
			if(get_zone(side, stor_to_hex(side, new_stor(i, j))) == CENTER){
				b[i][j] = EMPTY;
			}else{
				b[i][j] = INVALID;
			}
		}
	}

	return b;
}

///\brief Returns board height.
int boardh(const int side){
	return 4*side-3;
}

///\brief Returns width of given row (Stor::i) in a star of given length.
int linew(const int side, int row){
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

///\brief Returns the zone wherein given hex is located.
Zone get_zone(const int side, Hex h){
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

///\brief Returns opposite zone
Zone get_opposite(Zone zn){
	Zone ret;
	switch(zn){
	case TOP:
		ret = BOT;
		break;
	case TOP_LEFT:
		ret = BOT_RIGHT;
		break;
	case BOT_LEFT:
		ret = TOP_RIGHT;
		break;
	case BOT:
		ret = TOP;
		break;
	case BOT_RIGHT:
		ret = TOP_LEFT;
		break;
	case TOP_RIGHT:
		ret = BOT_LEFT;
		break;
	default:
		ret = NOWHERE;
		break;
	}
	return ret;
}

///\brief Returns number of cells in a star corner
int get_corner_size(const int side){
	return (side*side-side)/2;
}

/**
 * \brief Returns a Hex array containing coordinates of cells in given corner
 *
 * \param zone Must be a corner, that is, neither CENTER nor NOWHERE
 */
Hex* get_corner_array(const int side, Zone corner){
	//Wrong input
	if(corner == CENTER || corner == NOWHERE) return NULL;

	//Correct input
	Hex cur; //Currently seen cell
	Hex to_next_line; //What to add to pass to the next time
	Hex to_next_in_line; //How to go to the next cell in current line
	Hex* hexes = (Hex*) malloc(get_corner_size(side)*sizeof(Hex));
	switch(corner){
	case TOP:
		cur = new_hex(-2*(side-1), side-1);
		to_next_line = new_hex(1, -1);
		to_next_in_line = new_hex(0, 1);
		break;
	case TOP_LEFT:
		cur = new_hex(1-side, 1-side);
		to_next_line = new_hex(1, 0);
		to_next_in_line = new_hex(-1, 1);
		break;
	case BOT_LEFT:
		cur = new_hex(side-1, -2*(side-1));
		to_next_line = new_hex(0, 1);
		to_next_in_line = new_hex(-1, 0);
		break;
	case BOT:
		cur = new_hex(2*(side-1), 1-side);
		to_next_line = new_hex(-1, 1);
		to_next_in_line = new_hex(0, -1);
		break;
	case BOT_RIGHT:
		cur = new_hex(side-1, side-1);
		to_next_line = new_hex(-1, 0);
		to_next_in_line = new_hex(1, -1);
		break;
	case TOP_RIGHT:
		cur = new_hex(1-side, 2*(side-1));
		to_next_line = new_hex(0, -1);
		to_next_in_line = new_hex(1, 0);
		break;
	default:
		//This is not supposed to happen.
		break;
	}
	hexes[0] = cur;
	int line_len = 1;
	int pos_in_line = 1;
	int i;
	//Iter as many times as cells in corner
	for(i=1; i<get_corner_size(side); i++){
		if(pos_in_line >= line_len){
			//If end of current line:
			//Go back to beginning of line
			cur.r -= to_next_in_line.r*(line_len-1);
			cur.q -= to_next_in_line.q*(line_len-1);
			//Go to next line
			cur.r += to_next_line.r;
			cur.q += to_next_line.q;
			//Update line status variables
			line_len++;
			pos_in_line = 1;
		}else{
			//If not end of current line:
			//Go to next cell in line
			cur.r += to_next_in_line.r;
			cur.q += to_next_in_line.q;
			//Update line status variable
			pos_in_line++;
		}
		hexes[i] = cur;
	}
	return hexes;
}

///\brief Sets every cell in given star corner to given content value
void set_corner_ct(Content** b, const int side, Zone zn, Content ct){
	//Wrong input
	if(zn == CENTER || zn == NOWHERE) return;

	//Correct input
	Hex *hexes = get_corner_array(side, zn);
	int i;
	for(i=0; i<get_corner_size(side); i++){
		set_ct(b, side, hexes[i], ct);
	}
	free(hexes);
	return;
}

//Cell functions
///\brief Returns the content of given hex on board
Content get_ct(Content** b, const int side, Hex h){
	if(get_zone(side, h) == NOWHERE) return INVALID;
	Stor s = hex_to_stor(side, h);
	return b[s.i][s.j];
}

///\brief Sets the content of given hex
void set_ct(Content** b, const int side, Hex h, Content c){
	Stor s = hex_to_stor(side, h);
	b[s.i][s.j] = c;
	return;
}

///\brief Returns distance between two hexes
int distance(Hex h1, Hex h2){
	Cube c1 = hex_to_cube(h1);
	Cube c2 = hex_to_cube(h2);
	return (abs(c1.x-c2.x)+abs(c1.y-c2.y)+abs(c1.z-c2.z))/2;
}

///\brief Returns an array of given Hex’s neighboring cells and sets len accordingly.
Hex* neighbors(const int side, Hex h, int* len){
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

///\brief Returns boolean (0/1) expressing if given Hex is in given Hex array.
int in_cell_array(Hex h, int len, Hex* arr){
	int i = 0;
	int ret = 0;
	while(i<len && !ret){
		ret = (h.r == arr[i].r) && (h.q == arr[i].q);
		i++;
	}
	return ret;
}

