#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

/* User Interface
 * The functions described here must not depend on the technology used in the UI.
 */

#include <ncurses.h>
#include "data_struct.h"

//Data
static const int CTRLS_RIGHT     = 'n';
static const int CTRLS_TOP_RIGHT = 'l';
static const int CTRLS_TOP_LEFT  = 'r';
static const int CTRLS_LEFT      = 't';
static const int CTRLS_BOT_LEFT  = 'd';
static const int CTRLS_BOT_RIGHT = 's';
static const int CTRLS_OK        = ' ';
static const int CTRLS_BACK      = 'v';
static const int CTRLS_EXIT      = 'q';

//General UI functions
void ui_init();
void ui_terminate();

//Screen coordinates
typedef struct{
	int y;
	int x;
} Scryx;
Scryx new_scryx(int, int);
Scryx hex_to_scryx(const int, Hex);
Scryx center_coordinates(const int);
void sc_move(Scryx);

//Board generals
void print_board(Content**, const int);

//Cells
void link(const int, Hex, Hex, int);

//User interaction
Hex move_cursor(Content**, const int, Hex);

#endif

