#include "text_ui.h"

//General UI functions
///\brief UI initialization
UI* ui_init(){
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

	//Windows
	UI* ui = (UI*) malloc(sizeof(UI));
	ui->main_win = newwin(LINES, COLS, 0, 0);
	box(ui->main_win, 0, 0);
	wrefresh(ui->main_win);
	return ui;
}

///\brief UI termination
void ui_terminate(UI* ui){
	wborder(ui->main_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(ui->main_win);
	delwin(ui->main_win);
	endwin();
	return;
}

//Screen coordinates
Scryx new_scryx(int y, int x){
	Scryx sc;
	sc.y = y;
	sc.x = x;
	return sc;
}

Scryx hex_to_scryx(UI* ui, Hex h){
	Scryx ctr = center_coordinates(ui);
	return new_scryx(ctr.y + h.r*2, ctr.x + h.q*4 + h.r*2);
}

Scryx center_coordinates(UI* ui){
	int h, w;
	getmaxyx(ui->main_win, h, w);
	return new_scryx(h/2, w/2);
}

void sc_move(UI* ui, Scryx sc){
	wmove(ui->main_win, sc.y, sc.x);
	return;
}

//Board
///\brief Prints board properly
void print_board(UI* ui, Content** b, const int side){
	int i, j;
	Scryx cell;
	for(i=0; i<boardh(side); i++){
		for(j=0; j<linew(side, i); j++){
			//For each cell on the board:
			//Get Hex coordinates
			Hex h = stor_to_hex(side, new_stor(i, j));

			//Set screen coordinates
			cell = hex_to_scryx(ui, h);

			//Print cell
			sc_move(ui, cell);
			Content ct = get_ct(b, side, h);
			wattron(ui->main_win, COLOR_PAIR(ct));
			if(get_ct(b, side, h) != EMPTY){
				wattron(ui->main_win, A_BOLD);
				waddch(ui->main_win, 'O');
				wattroff(ui->main_win, A_BOLD);
			}else{
				waddch(ui->main_win, '.');
			}
			wattroff(ui->main_win, COLOR_PAIR(ct));
		}
	}
	wrefresh(ui->main_win);

	return;
}

//Cells
/**
 * \brief Prints or erases a link between adjacent hexes on the board.
 * 
 * \param h1 First Hex to link
 * \param h2 Second Hex to link
 * \param mode If 1: print link; 0: erase link.
 */
void link(UI* ui, const int side, Hex h1, Hex h2, int mode){
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

	Scryx sc1 = hex_to_scryx(ui, h1);
	Scryx sc2 = hex_to_scryx(ui, h2);

	wmove(ui->main_win, (sc1.y+sc2.y)/2, (sc1.x+sc2.x)/2);
	waddch(ui->main_win, link_char);
	wrefresh(ui->main_win);
	return;
}

//User interaction
///\brief Interactively moves cursor on the board
Hex move_cursor(UI* ui, Content** b, const int side, Hex curs_h){
	int cont = 1;
	Hex next;
	Content ct;
	int ch;
	while(cont){
		//Print cursor
		sc_move(ui, hex_to_scryx(ui, curs_h));
		ct = get_ct(b, side, curs_h);
		wattrset(ui->main_win, COLOR_PAIR(ct) | A_REVERSE);
		if(ct != EMPTY){
			waddch(ui->main_win, 'O');
		}else{
			waddch(ui->main_win, '.');
		}
		wrefresh(ui->main_win);

		//Get user input and move
		ch = wgetch(ui->main_win);
		next = curs_h;
		if(ch == CTRLS_RIGHT){
			next.q++;
		}else if(ch == CTRLS_TOP2){
			next.r--;
			next.q++;
		}else if(ch == CTRLS_TOP1){
			next.r--;
		}else if(ch == CTRLS_LEFT){
			next.q--;
		}else if(ch == CTRLS_BOT2){
			next.r++;
			next.q--;
		}else if(ch == CTRLS_BOT1){
			next.r++;
		}else if(ch == CTRLS_OK){
			cont = 0;
		}
		
		//If next is correct, erase and move
		if(get_zone(side, next) != NOWHERE){
			sc_move(ui, hex_to_scryx(ui, curs_h));
			wattroff(ui->main_win, A_REVERSE);
			if(ct != EMPTY){
				wattron(ui->main_win, A_BOLD);
				waddch(ui->main_win, 'O');
			}else{
				wattron(ui->main_win, A_NORMAL);
				waddch(ui->main_win, '.');
			}
			wrefresh(ui->main_win);
			curs_h = next;
		}
	}
	wstandend(ui->main_win);
	return curs_h;
}

//Not board or cell related
/**
 * \brief A menu from which the user is able to choose
 * 
 * \param title The menu title
 * \param len number of possible choices, that is, length of items[]
 * \param items[] array of string to be chosen from
 */
int choice_menu(UI* ui, char titre[], int len, char* items[]){
	return 0;
}

