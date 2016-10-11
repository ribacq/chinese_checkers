#include <stdlib.h>
#include <stdio.h>
#include "data_struct.h"
#include "text_ui.h"

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
	print_board(b, side);
	
	//End
	//free_board(b, side);
	ui_terminate();
	return EXIT_SUCCESS;
}

