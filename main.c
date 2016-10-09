#include <stdlib.h>
#include <stdio.h>
#include "data_struct.h"
#include "text_ui.h"

/* Chinese Checkers main program
 */

//Constants
const int BOARD_SIDE = 2;

int main(int argc, char* argv[])
{
	//UI init
	ui_init();

	//Board test
	Content** b = init_board(BOARD_SIDE);
	print_board(b, BOARD_SIDE);
	refresh();
	
	//End
	ui_terminate();
	return 0;
}

