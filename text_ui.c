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
	
	//Colors
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
	/* UI termination
	 */
	refresh();
	endwin();
	return;
}

//Screen coordinates
Scryx new_scryx(int y, int x){
	/* Returns Scryx with given values
	 */
	Scryx sc;
	sc.y = y;
	sc.x = x;
	return sc;
}

Scryx hex_to_scryx(const int side, Hex h){
	/* Hex to Scryx conversion
	 */
	Scryx ctr = center_coordinates(side);
	return new_scryx(ctr.y + h.r*2, ctr.x + h.q*4 + h.r*2);
}

Scryx center_coordinates(const int side){
	/* Returns Scryx containing the coordinates of the board’s center
	 */
	return new_scryx(4*(side-1), 6*(side-1));
}

void sc_move(Scryx sc){
	/* Moves cursor to sc
	 */
	move(sc.y, sc.x);
	return;
}

//Board
void print_board(Content** b, const int side){
	/* Prints board porperly
	 */
		
	int i, j;
	Scryx cell;
	for(i=0; i<boardh(side); i++){
		for(j=0; j<linew(side, i); j++){
			//For each cell on the board:
			//Get Hex coordinates
			Hex h = stor_to_hex(side, new_stor(i, j));

			//Set screen coordinates
			cell = hex_to_scryx(side, h);

			//Print cell
			sc_move(cell);
			Content ct = get_ct(b, side, h);
			attron(COLOR_PAIR(ct));
			if(get_ct(b, side, h) != EMPTY){
				attron(A_BOLD);
				addch('O');
				attroff(A_BOLD);
			}else{
				addch('.');
			}
			attroff(COLOR_PAIR(ct));
		}
	}
	refresh();

	return;
}

//Cells
void link(const int side, Hex h1, Hex h2, int mode){
	/* Prints or erases a link between adjacent hexes on the board.
	 * mode = 1: print
	 * mode = 0: erase
	 */

	//Wrong input: not on the board
	if(get_zone(side, h1) == NOWHERE || get_zone(side, h2) == NOWHERE) return;
	//Wrong input: not neighbors
	if(distance(h1, h2) != 1) return;
	
	//Input is correct.
	Cube c1 = hex_to_cube(h1);
	Cube c2 = hex_to_cube(h2);
	
	char link_char;
	if(!mode){
		link_char = ' ';
	}else if(c1.x == c2.x){
		link_char = '\\';
	}else if(c1.y == c2.y){
		link_char = '/';
	}else{ //c1.z == c2.z
		link_char = '-';
	}

	Scryx sc1 = hex_to_scryx(side, h1);
	Scryx sc2 = hex_to_scryx(side, h2);

	move((sc1.y+sc2.y)/2, (sc1.x+sc2.x)/2);
	addch(link_char);
	refresh();
	return;
}

//User interaction
Hex move_cursor(Content** b, const int side, Hex curs_h){
	/* Interactively moves cursor on the board
	 */
	int cont = 1;
	Hex next;
	Content ct;
	int ch;
	while(cont){
		//Print cursor
		sc_move(hex_to_scryx(side, curs_h));
		ct = get_ct(b, side, curs_h);
		attrset(COLOR_PAIR(ct) | A_REVERSE);
		if(ct != EMPTY){
			addch('O');
		}else{
			addch('.');
		}
		refresh();

		//Get user input and move
		ch = getch();
		if(ch == CTRLS_LEFT){
			next = new_hex(curs_h.r, curs_h.q-1);
		}else if(ch == CTRLS_DOWN){
			next = new_hex(curs_h.r+1, curs_h.q);
		}else if(ch == CTRLS_UP){
			next = new_hex(curs_h.r-1, curs_h.q);
		}else if(ch == CTRLS_RIGHT){
			next = new_hex(curs_h.r, curs_h.q+1);
		}else if(ch == CTRLS_OK){
			cont = 0;
		}
		
		//If next is correct, erase and move
		if(get_zone(side, next) != NOWHERE){
			sc_move(hex_to_scryx(side, curs_h));
			attroff(A_REVERSE);
			if(ct != EMPTY){
				attron(A_BOLD);
				addch('O');
			}else{
				attron(A_NORMAL);
				addch('.');
			}
			refresh();
			curs_h = next;
		}
	}
	standend();
	return curs_h;
}

