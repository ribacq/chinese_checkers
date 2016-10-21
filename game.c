#include "game.h"

//Static functions
static void init_player_color(UI *ui, Player *plr, Content availables[6]);

/**
 * \brief Players setup
 *
 * Asks the user how many players there must be, and for each of them, their
 * name, and if the number of players allow it, their favorite colors.
 * Also, it’s the ONLY function to make calls to init_player_color().
 */
Player* init_players(UI* ui, int* nb_players){
	//Number of players
	char* menu_title = "How many players will there be?";
	char* menu_items[] = { "2 Players", "3 Players", "4 Players", "6 Players" };
	int nb_items = 4;
	int menu_res = choice_menu(ui, menu_title, nb_items, menu_items);
	*nb_players = 2+menu_res+(menu_res == 3);
	
	//Colors and goals of players
	Zone goals[*nb_players];
	if(*nb_players == 2){
		goals[0] = TOP;
		goals[1] = BOT;
	}else if(*nb_players == 3){
		goals[0] = TOP;
		goals[1] = BOT_LEFT;
		goals[2] = BOT_RIGHT;
	}else if(*nb_players == 4){
		goals[0] = TOP_LEFT;
		goals[1] = BOT_LEFT;
		goals[2] = BOT_RIGHT;
		goals[3] = TOP_RIGHT;
	}else{		
		goals[0] = TOP;	
		goals[1] = TOP_LEFT;
		goals[2] = BOT_LEFT;
		goals[3] = BOT;
		goals[4] = BOT_RIGHT;
		goals[5] = TOP_RIGHT;
	}
	
	//First player
	Content availables[6] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
	Player *p1 = (Player*) malloc(sizeof(Player));
	p1->goal = goals[0];
	p1->curs_h = new_hex(0, 0);
	ui_prompt_string(ui, p1->name, "Please enter first player name:");
	init_player_color(ui, p1, availables);
	
	//And the rest of them
	Player *cur, *prev = p1;
	int i;
	for(i=1; i<*nb_players; i++){
		cur = (Player*) malloc(sizeof(Player));
		prev->next = cur;
		cur->goal = goals[i];
		cur->curs_h = new_hex(0, 0);
		ui_prompt_string(ui, cur->name, "Please enter next player name:");
		init_player_color(ui, cur, availables);
		prev = cur;
	}
	cur->next = p1;
	return p1;
}

/**
 * \brief Manages the color choice menus during the Player objects creation
 *
 * This is a static function, called ONLY by init_players().
 * 
 * \param *plr the current player
 * \param availables[6] must be initialized to
 *	{RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN}
 *	before first call of this function.
 */
static void init_player_color(UI *ui, Player *plr, Content availables[6]){
	//Initial menu setup
	char menu_title[50];
	sprintf(menu_title, "%s, choose your color:", plr->name);
	char *menu_items[6];
	menu_items[0] = calloc(20, sizeof(char));
	strcpy(menu_items[0], "Red");
	menu_items[1] = calloc(20, sizeof(char));
	strcpy(menu_items[1], "Green");
	menu_items[2] = calloc(20, sizeof(char));
	strcpy(menu_items[2], "Yellow");
	menu_items[3] = calloc(20, sizeof(char));
	strcpy(menu_items[3], "Blue");
	menu_items[4] = calloc(20, sizeof(char));
	strcpy(menu_items[4], "Magenta");
	menu_items[5] = calloc(20, sizeof(char));
	strcpy(menu_items[5], "Cyan");

	//Remove not available from menu_items
	int i;
	for(i=0; i<6; i++){
		if((availables[i] == EMPTY) || (availables[i] == INVALID)){
			strcpy(menu_items[i], "N/A");
		}
	}
	
	//Finally use the menu in a loop
	int choice;
	do{
		choice = choice_menu(ui, menu_title, 6, menu_items);
	}while(strcmp(menu_items[choice], "N/A") == 0);
	
	//Use result
	plr->ct = availables[choice];
	availables[choice] = INVALID;
	return;
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
		if(get_ct(b, side, from) == plr->ct && ui->signal != QUIT){
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

