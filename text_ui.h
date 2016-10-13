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
Scryx hex_to_scryx(const int, Hex);
Scryx center_coordinates(const int);
void sc_move(Scryx);

//Board generals
void print_board(Content**, const int);

//Cells
int link(const int, Hex, Hex);

#endif

