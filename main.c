#include <stdlib.h>
#include "data_struct.h"
#include "text_ui.h"
#include "game.h"

/**
 * \mainpage Chinese Checkers in C
 *
 * This software uses `ncurses.h`.
 */

/**
 * \file main.c
 * \brief Main program for Chinese Checkers
 *
 * This file is where the main() function lives. It’s supposed to be fairly
 * independant from how other files in the project work, especially UI
 * related.
 */

/** \brief Default board side length. */
const int DEF_BOARD_SIDE = 5;

/**
 * \brief Chinese Checkers main function
 *
 * \param argc Number of command-line arguments
 * \param argv String values of command-line arguments.
 *
 * If a number is passed to the program as argument command-line argument, its
 * value will be used as side.
 */
int main(int argc, char* argv[]){
	//UI init
	UI* ui = ui_init();

	//Board test
	int side = DEF_BOARD_SIDE;
	if(argc == 2){
		side = strtol(argv[1], NULL, 10);
	}
	Content** b = init_board(side);
	set_ct(b, side, new_hex(2, -1), CYAN);
	set_ct(b, side, new_hex(0, -1), YELLOW);
	set_ct(b, side, new_hex(0, 0), RED);
	set_ct(b, side, new_hex(2, -2), BLUE);
	set_ct(b, side, new_hex(-1, 0), MAGENTA);
	set_ct(b, side, new_hex(-2, 1), GREEN);
	print_board(ui, b, side);
	
	//*
	int nb;
	Hex curs_h = move_cursor(ui, b, side, new_hex(0, 0));
	Hex tmp_h = curs_h;
	Hex cells[side*side];
	while(curs_h.r != -2*(side-1) || curs_h.q != side-1){
		nb = 1;
		cells[0] = curs_h;
		link_accessible_from(ui, b, side, &nb, cells, 1);
		tmp_h = move_cursor(ui, b, side, curs_h);
		nb = 1;
		cells[0] = curs_h;
		link_accessible_from(ui, b, side, &nb, cells, 0);
		curs_h = tmp_h;
	}
	//*/
	
	//End
	ui_terminate(ui);
	return EXIT_SUCCESS;
}

