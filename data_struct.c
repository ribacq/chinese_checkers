#include "data_struct.h"

/*
 * Hexes are located with cube coordinates.
 * Ressource used: http://www.redblobgames.com/grids/hexagons
 */

//Coordinates conversion
Cube hex_to_cube(Hex h){
	Cube c;
	c.x = h.r;
	c.y = h.q;
	c.z = 0 - h.r - h.q;
	return c;
}

Hex cube_to_hex(Cube c){
	Hex h;
	h.r = c.x;
	h.q = c.y;
	return h;
}

//Board
const char DISP_CHARS[8] = {'o', 'r', 'g', 'b', 'y', 'w', 'p', ' '};

Content* init_board(int side){
	/* Contents are stored in a linear table initialized with by default
	 * FORBIDDEN in the corners and EMPTY elsewhere, in order for the
	 * board to be an hexagon.
	 */

	int dim = 2*side-1;
	Content* b = (Content*) malloc(sizeof(Content)*dim*dim);
	int i, j;
	for(i=0; i<dim; i++){
		for(j=0; j<dim; j++){
			if(j<side-1-i || j>3*side-3-i){
				b[i*dim+j] = FORBIDDEN;
			}else{
				b[i*dim+j] = EMPTY;
			}
		}
	}
	return b;
}

void test_print_board(Content* b, int side){
	/* Test prints the board to standard output using the format below:
	 * For example if side is 4:
	 *
	 *     0   1   2   3   4   5   6
	 * 0 |   |   |   | - | - | - | - |
	 * 1 |   |   | - | - | - | - | - |
	 * 2 |   | - | - | - | - | - | - |
	 * 3 | - | - | - | - | - | - | - |
	 * 4 | - | - | - | - | - | - |   |
	 * 5 | - | - | - | - | - |   |   |
	 * 6 | - | - | - | - |   |   |   |
	 */

	int dim = 2*side-1;
	int i, j;
	for(i=0; i<dim; i++){
		printw("|");
		for(j=0; j<dim; j++){
			printw(" ");
			if(b[i*dim+j] == FORBIDDEN){
				printw("#");
			}else if(b[i*dim+j] == EMPTY){
				printw("-");
			}else{
				printw(" ");
			}
			printw(" |");
		}
		printw("\n");
	}
	return;
}

void print_board(Content* b, int side){
	/* Prints board porperly
	 */

	int dim = 2*side-1;
	int i, j, k;
	for(i=0; i<dim; i++){
		//For each line, display first the elements
		for(k=0; k<side-1-i || k<i-side+1; k++){
			//Line-indent
			printw("  ");
		}
		for(j=0; j<dim; j++){
			//Display of rows
			if(b[i*dim+j] != FORBIDDEN){
				printw("%c", DISP_CHARS[b[i*dim+j]]);
				if(j<dim-1 && b[i*dim+j+1] != FORBIDDEN){
					printw("---");
				}
			}
		}
		printw("\n");
		//And on the next line, the slashes and backslashes connectors
		if(i<dim-1){
			for(k=0; k<side-2-i || k<i-side+1; k++){
				//Line-indent
				printw("  ");
			}
			printw(" ");
			for(j=0; j<dim-1; j++){
				if(b[i*dim+j+1] != FORBIDDEN){
					if(i<side-1){
						printw("/ \\ ");
					}else{
						printw("\\ / ");
					}
				}
			}
			printw("\n");
		}
	}

	return;
}

