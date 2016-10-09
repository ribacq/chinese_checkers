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
	int y_c = side*2-2;
	int x_c = side*4-4;
	
	int i, j, y, x;
	for(i=0; i<2*side-1; i++){
		for(j=0; j<linew(side, i); j++){
			Stor s; s.i = i; s.j = j;
			Hex h = stor_to_hex(side, s);
			y = y_c + h.r*2;
			x = x_c + h.q*4 + h.r*2;
			move(y, x);
			addch(DISP_CHARS[get_ct(b, side, h)]);
		}
	}
	mvaddch(y_c, x_c, '#');

	return;
}

