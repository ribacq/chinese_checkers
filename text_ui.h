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
#define CTRLS_RIGHT 'n'
#define CTRLS_TOP2  'l'
#define CTRLS_TOP1  'r'
#define CTRLS_LEFT  't'
#define CTRLS_BOT2  'd'
#define CTRLS_BOT1  's'
#define CTRLS_OK    ' '
#define CTRLS_BACK  'v'
#define CTRLS_QUIT  'q'
///\}

///\defgroup general_text_ui_gp General Text UI functions and data
///\{

///\brief User Interface data structure
typedef struct{
	WINDOW* main_win; ///< \brief Pointer to main curses window.
	enum { CONTINUE, PAUSE, QUIT } signal; ///< \brief message to the outside.
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

//Not board or cell related
int choice_menu(UI*, char*, int, char**);

//Board generals
void print_board(UI*, Content**, const int);

//Cells
void link(UI*, const int, Hex, Hex, int);

//User interaction
Hex move_cursor(UI*, Content**, const int, Hex);

#endif

