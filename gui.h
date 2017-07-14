/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

/* Graphical User Interface (SDL)
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "data_struct.h"

//color conversions
#define HEX2MAP(color) SDL_MapRGB(ui->main_surf->format, (0xff0000 & color) >> 16, (0x00ff00 & color) >> 8, (0x0000ff & color))
#define HEX2STRUCT(color) (SDL_Color) { .r = (0xff0000 & color) >> 16, .g = (0x00ff00 & color) >> 8, .b = (0x0000ff & color), .a = 0xff }

/**
 * \file gui.h
 * \brief Graphical User Interface
 *
 * The functions and data here use the SDL library to create graphical windows.
 */

/**
 * \defgroup general_gui_gp General GUI functions and data
 * \{
 */

/**
 * \brief User Interface data structure
 */
typedef struct {
	SDL_Window *main_win;           /**< \brief Pointer to main window */
	SDL_Surface *main_surf;         /**< \brief Main window’s surface */
	SDL_Rect main_rect;             /**< \brief window surface size */
	SDL_Surface *cell_tiles[8];     /**< \brief Cell images */
	SDL_Surface *selected_mask;     /**< \brief a white border for a single cell */
	SDL_Surface *unselected_mask;   /**< \brief a black border for a single cell */
	int cell_colors[8];             /**< \brief the colors in the game */
	int bg_color;                   /**< \brief window background color */
	int border_color;               /**< \brief hex border color, used for text */
	SDL_Rect hex_rect;              /**< \brief default rect for a hexagon with default width and height */
	TTF_Font *font;                 /**< \brief font used to display text */
	enum { CONTINUE, QUIT } signal; /**< \brief message to the outside */
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
 * \brief Hex to SDL_Rect conversion
 */
SDL_Rect hex_to_rect(UI *ui, Hex h);

/**
 * \brief Coordinates of screen center
 */
SDL_Rect center_coordinates(UI *ui);

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

