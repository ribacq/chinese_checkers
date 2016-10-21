#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

/** \file game.h
 * \brief Functions and data related to the game process
 */

#include <stdlib.h>
#include <string.h>
#include "data_struct.h"
#include "text_ui.h"

///\brief Data struct to represent the players
typedef struct player_struct{
	Content ct; ///< \brief Displayed color and pieces identifier
	Zone goal; ///< \brief Where the player’s pieces must go
	Hex curs_h; ///< \brief Player’s personal cursor
	char name[20]; ///< \brief Player’s name
	struct player_struct* next; ///< \brief Pointer to next player to play
} Player;

Player* init_players(UI*, int*);
void link_accessible_from(UI*, Content**, const int, int*, Hex[], int);
void play_turn(UI*, Content**, const int, Player*);
int has_won(Content**, const int, Player*);

#endif

