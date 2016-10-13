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
	getch();
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
int link(const int side, Hex h1, Hex h2){
	/* Prints a link between adjacent hexes on the board.
	 * Returns 1 if success, 0 if failure.
	 */

	//Wrong input: not on the board
	if(get_zone(side, h1) == NOWHERE || get_zone(side, h2) == NOWHERE) return 0;
	//Wrong input: not neighbors
	if(distance(h1, h2) != 1) return 0;
	
	//Input is correct.
	Cube c1 = hex_to_cube(h1);
	Cube c2 = hex_to_cube(h2);
	
	char link_char;
	if(c1.x == c2.x){
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
	return 1;
}

