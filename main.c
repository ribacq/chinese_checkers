#include <stdlib.h>
#include "data_struct.h"
#include "text_ui.h"
#include "game.h"

/* Chinese Checkers main program
 */

//Constants
const int DEF_BOARD_SIDE = 5;

int main(int argc, char* argv[]){
	//UI init
	ui_init();

	//Board test
	int side = DEF_BOARD_SIDE;
	if(argc == 2){
		side = strtol(argv[1], NULL, 10);
	}
	Content** b = init_board(side);
	Hex h = new_hex(3, -1);
	set_ct(b, side, h, CYAN);
	set_ct(b, side, new_hex(0, -1), YELLOW);
	set_ct(b, side, new_hex(0, 0), RED);
	set_ct(b, side, new_hex(2, -1), BLUE);
	set_ct(b, side, new_hex(-1, 0), MAGENTA);
	set_ct(b, side, new_hex(-2, 1), GREEN);
	print_board(b, side);
	int nb = 1;
	int total_nb = 3*side*side-2*side+1;
	Hex cells[total_nb];
	cells[0] = h;
	link_accessible_from(b, side, &nb, cells);
	
	//End
	ui_terminate();
	return EXIT_SUCCESS;
}

