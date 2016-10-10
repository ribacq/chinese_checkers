#include "text_ui.h"

//General UI functions
void ui_init(){
	/* UI initialization
	 */

	initscr();		//Starts ncurses mode
	raw();			//Disables line buffering
	noecho();		//Disables input echo
	keypad(stdscr, TRUE);	//Enable special keys
	return;
}

void ui_terminate(){
	refresh();
	getch();
	endwin();
	return;
}

//Board
const char DISP_CHARS[8] = {'O', 'R', 'G', 'B', 'Y', 'W', 'P', ' '};
void print_board(Content** b, int side){
	/* Prints board porperly
	 */
	
	//Central hex coordinates on screen
	int y_c = side*4-4;
	int x_c = side*6-6;
	
	int i, j, y, x;
	for(i=0; i<boardh(side); i++){
		for(j=0; j<linew(side, i); j++){
			//For each cell on the board:
			//Get Hex coordinates
			Hex h = stor_to_hex(side, new_stor(i, j));

			//Set screen coordinates and print
			y = y_c + h.r*2;
			x = x_c + h.q*4 + h.r*2;
			move(y, x);
			addch(DISP_CHARS[get_ct(b, side, h)]);

			//Link with existing neighbors
			//Middle-Right
			if(exists(side, new_hex(h.r, h.q+1))){
				move(y, x+1);
				addstr(" - ");
			}

			//Bottom-Left
			if(exists(side, new_hex(h.r+1, h.q-1))){
				move(y+1, x-1);
				addch('/');
			}

			//Bottom-Right
			if(exists(side, new_hex(h.r+1, h.q))){
				move(y+1, x+1);
				addch('\\');
			}
		}
	}
	mvaddch(y_c, x_c, '#');
	move(boardh(side)*2, 0);
	refresh();

	return;
}

