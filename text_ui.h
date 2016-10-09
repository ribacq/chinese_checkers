#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#include <ncurses.h>
#include "data_struct.h"

/* User Interface
 * The functions described here must not depend on the technology used in the UI.
 */

//General UI functions
void ui_init();
void ui_terminate();

//Board
void print_board(Content**, int);

#endif

