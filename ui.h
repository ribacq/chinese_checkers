/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

/* Common User Interface header file
 */

#include "data_struct.h"

#ifdef _GUI
	#include "gui.h"
#else
	#include "text_ui.h"
#endif

/**
 * \file ui.h
 * \brief Common User Interface header file
 *
 * The functions and data here should be technology-independent.
 */

/**
 * \defgroup general_gui_gp General GUI functions and data
 * \{
 */

/**
 * \brief User Interface data structure
 */
typedef struct UI UI;

/**
 * \brief UI initialization
 */
UI *ui_init();

/**
 * \brief UI termination
 */
void ui_terminate(UI *ui);

/**
 * \brief Correctly clear screen
 */
void ui_clear(UI *ui);

/**
 * \}
 */

//Board generals
/**
 * \brief Prints board properly
 */
void print_board(UI *ui, Content **b, const int side);

//Cells
/**
 * \brief Prints or erases a link between adjacent hexes on the board.
 * 
 * \param h1 First Hex to link
 * \param h2 Second Hex to link
 * \param mode If 1: print link; 0: erase link.
 */
void link(UI *ui, const int side, Hex h1, Hex h2, int mode);

//User interaction
/**
 * \brief Interactively moves cursor on the board
 *
 * \param curs_h the old cursor position
 *
 * \return the new cursor position
 */
Hex move_cursor(UI *ui, Content **b, const int side, Hex curs_h);

//Not board or cell related
/**
 * \brief A menu from which the user is able to choose
 * 
 * \param title The menu title
 * \param len number of possible choices, that is, length of items
 * \param items array of string to be chosen from
 */
int choice_menu(UI *ui, char *title, const int len, char **items);

/**
 * \brief Prompts user for a string
 */
void ui_prompt_string(UI *ui, char *dst, const char *prompt);

/**
 * \brief Prints whose turn it is
 */
void print_status(UI *ui, Content ct, char *name);

/**
 * \brief Prints a message in a text box
 *
 * \param ct The color in which to display the message. It mustn’t be EMPTY.
 * \param msg The message to display
 */
void disp_msg(UI *ui, Content ct, char *msg);

#endif

