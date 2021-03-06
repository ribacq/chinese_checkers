/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

#include <stdlib.h>
#include <string.h>

#include "data_struct.h"
#include "ui.h"
#include "game.h"

/**
 * \mainpage Chinese Checkers in C
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

	//Set board side to default or conmand-line arg.
	int side = DEF_BOARD_SIDE;
	if(argc == 2){
		side = strtol(argv[1], NULL, 10);
	}
	Content** b;
	
	//Menu
	char* menu_title = "Chinese Checkers";
	char* menu_items[] = {"Play", "Quit"};
	int menu_len = sizeof(menu_items)/sizeof(menu_items[0]);

	//Print menu and do main loop
	while((ui->signal = choice_menu(ui, menu_title, menu_len, menu_items) == 0 ? CONTINUE : QUIT) != QUIT) {
		//Board
		b = init_board(side);
		
		//Players
		int nb_players;
		Player* cur_plr = init_players(ui, &nb_players);
		int i;
		for(i=0; i<nb_players; i++){
			set_corner_ct(b, side, get_opposite(cur_plr->goal), cur_plr->ct);
			if (nb_players == 3)
				set_corner_ct(b, side, cur_plr->goal, EMPTY);
			cur_plr = cur_plr->next;
		}

		//Loop
		print_board(ui, b, side);
		print_status(ui, cur_plr->ct, cur_plr->name);
		while(ui->signal != QUIT){
			play_turn(ui, b, side, cur_plr);
			if(has_won(b, side, cur_plr)){
				//Game Over
				char msg[20] = "";
				sprintf(msg, "%s wins!", cur_plr->name);
				disp_msg(ui, cur_plr->ct, msg);
				ui->signal = QUIT;
			}
			cur_plr = cur_plr->next;
			print_board(ui, b, side);
			print_status(ui, cur_plr->ct, cur_plr->name);
		}
		ui_clear(ui);
		free(b);
	}
//End
ui_terminate(ui);
return EXIT_SUCCESS;
}

