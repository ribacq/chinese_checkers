/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

/* Graphical User Interface (SDL)
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

//color conversions
#define HEX2MAP(color) SDL_MapRGB(ui->main_surf->format, (0xff0000 & color) >> 16, (0x00ff00 & color) >> 8, (0x0000ff & color))
#define HEX2STRUCT(color) (SDL_Color) { .r = (0xff0000 & color) >> 16, .g = (0x00ff00 & color) >> 8, .b = (0x0000ff & color), .a = 0xff }

//UI data structure
struct UI {
	enum { CONTINUE, QUIT } signal; /**< \brief Signal to the outside */
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
};

