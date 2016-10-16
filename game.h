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
	Content ct; ///< \brief Displayed color and pieces identifier.
	char name[10]; ///< \brief Player’s name
	struct player_struct* next; ///< \brief Pointer to next player to play.
} Player;

Player* init_players(UI*);
void link_accessible_from(UI*, Content**, const int, int*, Hex[], int);
void play_turn(UI*, Content**, const int, Player*);

#endif

