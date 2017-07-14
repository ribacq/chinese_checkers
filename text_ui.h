/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

/*
 * ncurses TUI
 */

#include <ncurses.h>

struct UI {
	enum { CONTINUE, QUIT } signal; /**< \brief Signal to the outside. */
	WINDOW *main_win;               /**< \brief Pointer to main curses window. */
};

