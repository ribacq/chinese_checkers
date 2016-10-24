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

///\defgroup controls_text_ui_gp Key Bindings in the Text UI
///\{
static const int CTRLS_LEFT  = KEY_LEFT;
static const int CTRLS_BOT   = KEY_DOWN;
static const int CTRLS_TOP   = KEY_UP;
static const int CTRLS_RIGHT = KEY_RIGHT;
static const int CTRLS_OK    = '\n';
static const int CTRLS_BACK  = 'v';
static const int CTRLS_QUIT  = 'q';
///\}

///\defgroup general_text_ui_gp General Text UI functions and data
///\{

///\brief User Interface data structure
typedef struct{
	WINDOW* main_win; ///< \brief Pointer to main curses window.
	enum { CONTINUE, QUIT } signal; ///< \brief message to the outside.
} UI;
UI* ui_init();
void ui_terminate(UI*);
void ui_clear(UI*);
///\}

///\brief Screen coordinates
typedef struct{
	int y; ///< \brief line number on screen (top-most = 0)
	int x; ///< \brief column number on screen (left-most = 0)
} Scryx;
///\brief Constructor for Scryx
Scryx new_scryx(int, int);
///\brief Hex to Scryx conversion
Scryx hex_to_scryx(UI*, Hex);
///\brief Coordinates of screen center
Scryx center_coordinates(UI*);
///\brief wmove equivalent, with Scryx parameter
void sc_move(UI*, Scryx);

//Board generals
void print_board(UI*, Content**, const int);

//Cells
void link(UI*, const int, Hex, Hex, int);
void move_piece(UI*, Content**, const int, Hex, Hex);

//User interaction
Hex move_cursor(UI*, Content**, const int, Hex);

//Not board or cell related
int choice_menu(UI*, char*, int, char**);
void ui_prompt_string(UI*, char*, const char*);
void print_status(UI*, Content, char*);
void disp_msg(UI*, Content, char*);

#endif

