#include "game.h"

/**
 * \brief Players setup
 *
 * Asks the user how many players there must be, and for each of them, their
 * name, and if the number of players allow it, their favorite colors.
 */
Player* init_players(UI* ui, int* nb_players){
	//Number of players
	char* menu_title = "How many players will there be?";
	char* menu_items[] = { "2 Players", "3 Players", "4 Players", "6 Players" };
	int nb_items = 4;
	int menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
	*nb_players = 2+menu_res+(menu_res == 3);
	
	//Colors and goals of players
	Content cts[*nb_players];
	Zone goals[*nb_players];
	if(*nb_players == 2){
		menu_title = "Please choose the playing colors:";
		menu_items[0] = "Red VS. Blue";
		menu_items[1] = "Green VS. Magenta";
		menu_items[2] = "Yellow VS. Cyan";
		nb_items = 3;
		menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
		if(menu_res == 0){
			cts[0] = RED;
			cts[1] = BLUE;
		}else if(menu_res == 1){
			cts[0] = GREEN;
			cts[1] = MAGENTA;
		}else{
			cts[0] = YELLOW;
			cts[1] = CYAN;
		}
		goals[0] = TOP;
		goals[1] = BOT;
	}else if(*nb_players == 3){
		menu_title = "Please choose the playing colors:";
		menu_items[0] = "Red VS. Yellow VS. Magenta";
		menu_items[1] = "Green VS. Blue VS. Cyan";
		nb_items = 2;
		menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
		if(menu_res == 0){
			cts[0] = RED;
			cts[1] = YELLOW;
			cts[2] = MAGENTA;
		}else{
			cts[0] = GREEN;
			cts[1] = BLUE;
			cts[2] = CYAN;
		}
		goals[0] = TOP;
		goals[1] = BOT_LEFT;
		goals[2] = BOT_RIGHT;
	}else if(*nb_players == 4){
		menu_title = "Please choose the playing colors:";
		menu_items[0] = "Green VS. Magenta VS. Yellow VS. Cyan";
		menu_items[1] = "Green VS. Magenta VS. Blue VS. Red";
		menu_items[2] = "Yellow VS. Cyan VS. Blue VS. Red";
		nb_items = 3;
		menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
		if(menu_res == 0){
			cts[0] = GREEN;
			cts[1] = YELLOW;
			cts[2] = MAGENTA;
			cts[3] = CYAN;
		}else if(menu_res == 1){
			cts[0] = GREEN;
			cts[1] = BLUE;
			cts[2] = MAGENTA;
			cts[3] = RED;
		}else if(menu_res == 2){
			cts[0] = YELLOW;
			cts[1] = BLUE;
			cts[2] = CYAN;
			cts[3] = RED;
		}
		goals[0] = TOP_LEFT;
		goals[1] = BOT_LEFT;
		goals[2] = BOT_RIGHT;
		goals[3] = TOP_RIGHT;
	}else{
		cts[0] = RED;
		goals[0] = TOP;
		cts[1] = GREEN;
		goals[1] = TOP_LEFT;
		cts[2] = YELLOW;
		goals[2] = BOT_LEFT;
		cts[3] = BLUE;
		goals[3] = BOT;
		cts[4] = MAGENTA;
		goals[4] = BOT_RIGHT;
		cts[5] = CYAN;
		goals[5] = TOP_RIGHT;
	}
	
	//Proper players
	Player *p1 = (Player*) malloc(sizeof(Player));
	p1->ct = cts[0];
	p1->goal = goals[0];
	p1->curs_h = new_hex(0, 0);
	ui_prompt_string(ui, p1->name, "Please enter first player name:");
	Player *cur, *prev = p1;
	int i;
	for(i=1; i<*nb_players; i++){
		cur = (Player*) malloc(sizeof(Player));
		prev->next = cur;
		cur->ct = cts[i];
		cur->goal = goals[i];
		cur->curs_h = new_hex(0, 0);
		ui_prompt_string(ui, cur->name, "Please enter next player name:");
		prev = cur;
	}
	cur->next = p1;
	return p1;
}

/**
 * \brief Display, erases and get the accessible cells
 * 
 * \param *nb pointer to the number of accessible cells
 *        *nb must equal 1 on function call from the outside.
 * \param cells[] array of coordinates to the accessible cells. Its first cell
 *        has to be initialized with the starting point.
 * \param mode if 1, link are drawn to show accessible cells; if 0, they’re
 *        erased.
 * 
 * Uses the link() function to make a display of cells currently accessible on
 * the board from the last hex in given array. In the case of a series of jumps,
 * the search makes the function call itself recursively.
 */
void link_accessible_from(UI* ui, Content** b, const int side, int* nb, Hex cells[], int mode){
	//Wrong input: source not on board
	if(get_zone(side, cells[*nb-1]) == NOWHERE) return;

	//Correct input
	int nb_neighs;
	Hex h = cells[*nb-1];
	Hex* neighs = neighbors(side, h, &nb_neighs);
	Hex tmp_h;
	int i, j, in_cells;
	for(i=0; i<nb_neighs; i++){
		if(get_ct(b, side, neighs[i]) != EMPTY){
			//Jump access is looked up wether or not we’re on our
			//source or not.
			tmp_h.r = h.r + 2*(neighs[i].r - h.r);
			tmp_h.q = h.q + 2*(neighs[i].q - h.q);
			if(get_ct(b, side, tmp_h) == EMPTY){
				link(ui, side, h, neighs[i], mode);
				link(ui, side, neighs[i], tmp_h, mode);
				in_cells = 0;
				for(j=0; j<*nb; j++)
					in_cells = in_cells || (tmp_h.r == cells[j].r && tmp_h.q == cells[j].q);
				if(!in_cells){
					cells[(*nb)++] = tmp_h;
					//Trace the rest of the path
					link_accessible_from(ui, b, side, nb, cells, mode);
				}
			}
		}else if(get_ct(b, side, h) != EMPTY){
			//Source is not empty:
			//This is the first cell from which we look, and only
			//now have we the right to go to the directly
			//neighboring cells
			link(ui, side, h, neighs[i], mode);
			cells[(*nb)++] = neighs[i];
		}
	}
	return;
}

/**
 * \brief Play someones turn
 */
void play_turn(UI* ui, Content** b, const int side, Player* plr){
	int nb;
	Hex possibilities[side*side];
	int move_made = 0;
	Hex from, to = plr->curs_h;
	while(!move_made && ui->signal != QUIT){
		from = move_cursor(ui, b, side, to);
		if(get_ct(b, side, from) == plr->ct){
			nb = 1; possibilities[0] = from;
			link_accessible_from(ui, b, side, &nb, possibilities, 1);
			to = move_cursor(ui, b, side, from);
			nb = 1; possibilities[0] = from;
			link_accessible_from(ui, b, side, &nb, possibilities, 0);
			if((to.r != from.r || to.q != from.q) && in_cell_array(to, nb, possibilities)){
				//Move the piece and launch next player’s turn
				move_piece(ui, b, side, from, to);
				move_made = 1;
			}
		}
	}
	plr->curs_h = to;
	return;
}

/**
 * \brief Returns a boolean (0/1) indicating whether given Player has won.
 *
 * A Player has won if all of their pieces are in their opposed star corner.
 */
int has_won(Content** b, const int side, Player* plr){
	int ret = 1;
	Hex* goal_arr = get_corner_array(side, plr->goal);
	int i = 0;
	while(ret && i<get_corner_size(side)){
		ret = get_ct(b, side, goal_arr[i]) == plr->ct;
		i++;
	}
	return ret;
}

