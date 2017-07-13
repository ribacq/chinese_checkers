#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

/* User Interface
 * The functions described here must not depend on the technology used in the UI.
 */

#include <ncurses.h>
#include "data_struct.h"

/**
 * \file text_ui.h
 * \brief Text User Interface definition
 *
 * The functions and data here use ncurses.h in order to display information in
 * the GNU/Linux terminal.
 */

/**
 * \defgroup controls_text_ui_gp Key Bindings in the Text UI
 * \{
 */
static const int CTRLS_LEFT  = KEY_LEFT;
static const int CTRLS_BOT   = KEY_DOWN;
static const int CTRLS_TOP   = KEY_UP;
static const int CTRLS_RIGHT = KEY_RIGHT;
static const int CTRLS_OK    = '\n';
static const int CTRLS_BACK  = 'v';
static const int CTRLS_QUIT  = 'q';
/**
 * \}
 */

/**
 * \defgroup general_text_ui_gp General Text UI functions and data
 * \{
 */

/**
 * \brief User Interface data structure
 */
typedef struct {
	WINDOW *main_win; /**< \brief Pointer to main curses window. */
	enum { CONTINUE, QUIT } signal; /**< \brief message to the outside. */
} UI;

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

/**
 * \brief Screen coordinates
 */
typedef struct {
	int y; /**< \brief line number on screen (top-most = 0) */
	int x; /**< \brief column number on screen (left-most = 0) */
} Scryx;

/**
 * \brief Constructor for Scryx
 */
Scryx new_scryx(int y, int x);

/**
 * \brief Hex to Scryx conversion
 */
Scryx hex_to_scryx(UI *ui, Hex h);

/**
 * \brief Coordinates of screen center
 */
Scryx center_coordinates(UI *ui);

/**
 * \brief wmove equivalent, with Scryx parameter
 */
void sc_move(UI *ui, Scryx sc);

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

/**
 * \brief Moves a piece on the board
 *
 * \param from Cell from which the piece is taken
 * \param to Cell to which the piece is taken
 *
 * It verifies if from really is a piece and to an empty cell, but not whether
 * the piece has the right to do the move.
 */
void move_piece(UI *ui, Content **b, const int side, Hex from, Hex to);

//User interaction
/**
 * \brief Interactively moves cursor on the board
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

