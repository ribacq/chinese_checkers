#include "text_ui.h"

//General UI functions
void ui_init(){
	/* UI initialization
	 */

	initscr();		//Starts ncurses mode
	raw();			//Disables line buffering
	noecho();		//Disables input echo
	curs_set(FALSE);	//Hides cursor
	keypad(stdscr, TRUE);	//Enable special keys
	if(has_colors() == FALSE){
		endwin();
		printf("Your terminal does not support color.\n");
		exit(1);
	}
	start_color();
	init_pair(EMPTY,	COLOR_WHITE,		COLOR_BLACK);
	init_pair(RED,		COLOR_RED,		COLOR_BLACK);
	init_pair(GREEN,	COLOR_GREEN,		COLOR_BLACK);
	init_pair(YELLOW,	COLOR_YELLOW,		COLOR_BLACK);
	init_pair(BLUE,		COLOR_BLUE,		COLOR_BLACK);
	init_pair(MAGENTA,	COLOR_MAGENTA,		COLOR_BLACK);
	init_pair(CYAN,		COLOR_CYAN,		COLOR_BLACK);
	return;
}

void ui_terminate(){
	refresh();
	getch();
	endwin();
	return;
}

//Board
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

			//Set screen coordinates
			y = y_c + h.r*2;
			x = x_c + h.q*4 + h.r*2;

			//Print cell
			move(y, x);
			Content ct = get_ct(b, side, h);
			attron(COLOR_PAIR(ct) | A_BOLD);
			addch('O');
			attroff(COLOR_PAIR(ct) | A_BOLD);

			//Link with existing neighbors
			//Middle-Right
			if(get_zone(side, new_hex(h.r, h.q+1)) != NOWHERE){
				move(y, x+1);
				addstr(" - ");
			}

			//Bottom-Left
			if(get_zone(side, new_hex(h.r+1, h.q-1)) != NOWHERE){
				move(y+1, x-1);
				addch('/');
			}

			//Bottom-Right
			if(get_zone(side, new_hex(h.r+1, h.q)) != NOWHERE){
				move(y+1, x+1);
				addch('\\');
			}
		}
	}
	refresh();

	return;
}

