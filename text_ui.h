#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

/* User Interface
 * The functions described here must not depend on the technology used in the UI.
 */

#include <ncurses.h>
#include "data_struct.h"

//General UI functions
void ui_init();
void ui_terminate();

//Screen coordinates
typedef struct scryx {
	int y;
	int x;
} Scryx;
Scryx new_scryx(int, int);
Scryx center_coordinates(int);
Scryx hex_to_scryx(int, Hex);
void sc_move(Scryx);

//Board generals
void print_board(Content**, int);

//Cells
int link(int, Hex, Hex);

#endif

