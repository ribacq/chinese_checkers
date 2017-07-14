/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

/**
 * \file game.h
 * \brief Functions and data related to the game process
 */

#include <stdlib.h>
#include <string.h>
#include "data_struct.h"
#ifdef _GUI
	#include "gui.h"
#else
	#include "text_ui.h"
#endif

/**
 * \brief Data struct to represent the players
 */
typedef struct player_struct {
	Content ct; /**< \brief Displayed color and pieces identifier */
	Zone goal; /**< \brief Where the player’s pieces must go */
	Hex curs_h; /**< \brief Player’s personal cursor */
	char name[20]; /**< \brief Player’s name */
	struct player_struct *next; /**< \brief Pointer to next player to play */
} Player;

/**
 * \brief Players setup
 *
 * Asks the user how many players there must be, and for each of them, their
 * name, and if the number of players allow it, their favorite colors.
 * Also, it’s the ONLY function to make calls to init_player_color().
 */
Player *init_players(UI *ui, int *nb_players);

/**
 * \brief Moves a piece on the board
 *
 * \param from Cell from which the piece is taken
 * \param to Cell to which the piece is taken
 *
 * It verifies if from really is a piece and to an empty cell, but not whether
 * the piece has the right to do the move.
 */
void move_piece(Content **b, const int side, Hex from, Hex to);

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
void link_accessible_from(UI *ui, Content **b, const int side, int *nb, Hex cells[], int mode);

/**
 * \brief Play someones turn
 */
void play_turn(UI *ui, Content **b, const int side, Player *plr);

/**
 * \brief Returns a boolean (0/1) indicating whether given Player has won.
 *
 * A Player has won if all of their pieces are in their opposed star corner.
 */
int has_won(Content **b, const int side, Player *plr);

#endif

