/*
 * © Quentin RIBAC, 2017
 * This is free software.
 * See LICENSE file for legal information.
 */

/* Graphical User Interface (SDL)
 */

#include "ui.h"

static Hex *linked_cells = NULL;
static int nb_linked_cells = 0;
static int is_linked(Hex h) {
	for (int i = 0; i < nb_linked_cells; i++)
		if (linked_cells[i].r == h.r && linked_cells[i].q == h.q)
			return 1;
	return 0;
}

UI *ui_init() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	
	UI *ui = (UI *) malloc(sizeof(UI));
	ui->signal = CONTINUE;
	
	ui->main_win = SDL_CreateWindow("Chinese Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
	ui->main_surf = SDL_GetWindowSurface(ui->main_win);
	SDL_GetClipRect(ui->main_surf, &(ui->main_rect));

	ui->cell_tiles[EMPTY]    = IMG_Load("assets/img/empty.png");
	ui->cell_tiles[RED]      = IMG_Load("assets/img/red.png");
	ui->cell_tiles[GREEN]    = IMG_Load("assets/img/green.png");
	ui->cell_tiles[YELLOW]   = IMG_Load("assets/img/yellow.png");
	ui->cell_tiles[BLUE]     = IMG_Load("assets/img/blue.png");
	ui->cell_tiles[MAGENTA]  = IMG_Load("assets/img/magenta.png");
	ui->cell_tiles[CYAN]     = IMG_Load("assets/img/cyan.png");
	ui->cell_tiles[INVALID]  = IMG_Load("assets/img/invalid.png");
	ui->selected_mask        = IMG_Load("assets/img/selected.png");
	ui->unselected_mask      = IMG_Load("assets/img/unselected.png");

	ui->cell_colors[EMPTY]   = 0x95a5a6;
	ui->cell_colors[RED]     = 0xe74c3c;
	ui->cell_colors[GREEN]   = 0x27ae60;
	ui->cell_colors[YELLOW]  = 0xf1c40f;
	ui->cell_colors[BLUE]    = 0x3498db;
	ui->cell_colors[MAGENTA] = 0x9b59b6;
	ui->cell_colors[CYAN]    = 0x1abc9c;
	ui->cell_colors[INVALID] = 0x34495e;
	ui->bg_color             = 0xbdc3c7;
	ui->border_color         = 0x2c3e50;

	SDL_GetClipRect(ui->cell_tiles[EMPTY], &(ui->hex_rect));

	ui->font = TTF_OpenFont("assets/fonts/FreeSerif.ttf", 16);

	ui_clear((UI *) ui);
	return (UI *) ui;
}

void ui_terminate(UI *ui) {
	ui_clear(ui);
	SDL_DestroyWindow(ui->main_win);
	for (int i = 0; i < 8; i++)
		SDL_FreeSurface(ui->cell_tiles[i]);
	SDL_FreeSurface(ui->selected_mask);
	SDL_FreeSurface(ui->unselected_mask);
	TTF_CloseFont(ui->font);
	free(linked_cells);
	free(ui);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void ui_clear(UI *ui) {
	SDL_FillRect(ui->main_surf, NULL, HEX2MAP(ui->bg_color));
	SDL_UpdateWindowSurface(ui->main_win);
}

static SDL_Rect center_coordinates(UI *ui) {
	SDL_Rect rect = ui->hex_rect;
	rect.x = (ui->main_rect.w - ui->hex_rect.w) * .5;
	rect.y = (ui->main_rect.h - ui->hex_rect.h) * .5;
	return rect;
}

static SDL_Rect hex_to_rect(UI *ui, Hex h) {
	SDL_Rect ctr = center_coordinates(ui);
	SDL_Rect rect = ui->hex_rect;
	rect.x = ctr.x + rect.w * (h.r * .5 + h.q);
	rect.y = ctr.y + rect.h * (h.r * .75);
	return rect;
}

void print_board(UI *ui, Content** b, const int side) {
	SDL_Rect rect;
	for (int i = 0; i < boardh(side); i++) {
		for (int j = 0; j < linew(side, i); j++) {
			//For each cell on the board:
			//Get Hex coordinates
			Hex h = stor_to_hex(side, new_stor(i, j));

			//Set screen coordinates
			rect = hex_to_rect(ui, h);

			//Print cell
			Content ct = get_ct(b, side, h);
			SDL_BlitSurface(ui->cell_tiles[ct], NULL, ui->main_surf, &rect);
		}
	}

	//show linked cells
	for (int i = 0; i < nb_linked_cells; i++) {
		rect = hex_to_rect(ui, linked_cells[i]);
		SDL_BlitSurface(ui->selected_mask, NULL, ui->main_surf, &rect);
	}
	SDL_UpdateWindowSurface(ui->main_win);
}

void link(UI *ui, const int side, Hex h1, Hex h2, int mode) {
	//Wrong input: not on the board
	if (get_zone(side, h1) == NOWHERE || get_zone(side, h2) == NOWHERE) return;
	//Wrong input: not neighbours
	if (distance(h1, h2) != 1) return;

	//Correct input
	if (mode) {
		//h1 and h2 will be added to linked_cells
		nb_linked_cells += 2;
		linked_cells = (Hex *) realloc(linked_cells, nb_linked_cells * sizeof(Hex));
		linked_cells[nb_linked_cells - 2] = h1;
		linked_cells[nb_linked_cells - 1] = h2;
	} else {
		//h1 and h2 will be removed from linked_cells ONCE
		int h1_found = 0;
		int h2_found = 0;
		int found_now = 0;
		for (int i = 0; i < nb_linked_cells; i++) {
			if (!h1_found && linked_cells[i].r == h1.r && linked_cells[i].q == h1.q) {
				h1_found = 1;
				nb_linked_cells--;
				found_now = 1;
			}
			if (!h2_found && linked_cells[i].r == h2.r && linked_cells[i].q == h2.q) {
				h2_found = 1;
				nb_linked_cells--;
				found_now = 1;
			}

			if (h1_found || h2_found) {
				linked_cells[i] = linked_cells[i + h1_found + h2_found];
			}

			if (found_now) {
				i--;
				found_now = 0;
			}
		}
		linked_cells = (Hex *) realloc(linked_cells, nb_linked_cells * sizeof(Hex));
	}
	SDL_Rect rect1 = hex_to_rect(ui, h1);
	SDL_Rect rect2 = hex_to_rect(ui, h2);
	SDL_Surface *mask = mode ? ui->selected_mask : ui->unselected_mask;
	SDL_BlitSurface(mask, NULL, ui->main_surf, &rect1);
	SDL_BlitSurface(mask, NULL, ui->main_surf, &rect2);
	SDL_UpdateWindowSurface(ui->main_win);
}

Hex move_cursor(UI *ui, Content **b, const int side, Hex curs_h) {
	char *menu_title = "Are you sure you want to quit?";
	char *menu_items[2] = {};
	menu_items[0] = "What, no, never!";
	menu_items[1] = "Yes of course, this game is not very good.";
	SDL_Rect curs_rect = hex_to_rect(ui, curs_h);
	SDL_Rect ctr = center_coordinates(ui);
	SDL_Event e;
	while (1) {
		SDL_WaitEvent(&e);
		if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_q) {
			if (choice_menu(ui, menu_title, 2, menu_items)) {
				ui->signal = QUIT;
				break;
			} else {
				print_board(ui, b, side);
			}
		} else if (e.type == SDL_QUIT) {
			ui->signal = QUIT;
			break;
		} else if (e.type == SDL_MOUSEMOTION) {
			//the mouse must be inside the window
			if (e.motion.x < curs_rect.w * .5 || e.motion.x > ui->main_rect.w - curs_rect.w * .5) continue;
			if (e.motion.y < curs_rect.h * .5 || e.motion.y > ui->main_rect.h - curs_rect.h * .5) continue;

			//erase previous cursor
			if (!is_linked(curs_h))
				SDL_BlitSurface(ui->unselected_mask, NULL, ui->main_surf, &curs_rect);

			//get hex from coordinates
			curs_h.r = floor((e.motion.y - ctr.y) / (ctr.h * .75));
			curs_h.q = floor((e.motion.x - ctr.x - (curs_h.r % 2) * ctr.w / 2) / (ctr.w * 1.0)) - curs_h.r / 2;
			
			//draw new cursor if it’s on the board
			if (get_zone(side, curs_h) != NOWHERE) {
				curs_rect = hex_to_rect(ui, curs_h);
				SDL_BlitSurface(ui->selected_mask, NULL, ui->main_surf, &curs_rect);
			}
		} else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == 1) {
			break;
		}
		SDL_UpdateWindowSurface(ui->main_win);
	}
	return curs_h;
}

/**
 * \brief static function drawing a hexagon and text alongside it, without updating the screen
 * \param x if -1, text will be centered horizontally
 * \param y if -1, text will be centered vertically
 *
 * \return an SDL_Rect containing the displayed hex and text
 */
static SDL_Rect hex_and_text(UI *ui, Content ct, int x, int y, const char *text) {
	const int x_centered = x < 0 || x > ui->main_rect.w;
	const int y_centered = y < 0 || y > ui->main_rect.h;
		
	//set hex
	SDL_Rect hex_rect = ui->hex_rect;
	hex_rect.x = x_centered ? -1 : x;
	hex_rect.y = y_centered ? -1 : y;

	//set text
	SDL_Surface *text_surf = TTF_RenderUTF8_Blended(ui->font, text, HEX2STRUCT(ui->cell_colors[ct]));
	SDL_Rect text_rect = { .x = -1, .y = -1, .w = -1, .h = -1 };
	if (text_rect.x != -1)
		SDL_FillRect(ui->main_surf, &text_rect, HEX2MAP(ui->bg_color));
	TTF_SizeUTF8(ui->font, text, &(text_rect.w), &(text_rect.h));
	text_rect.w = text_rect.w < 100 ? 100 : text_rect.w;

	//position hex and text
	if (x_centered)
		hex_rect.x = (ui->main_rect.w - hex_rect.w - text_rect.w) / 2;
	if (y_centered)
		hex_rect.y = (ui->main_rect.h - hex_rect.h - text_rect.h) / 2;
	text_rect.x = hex_rect.x + hex_rect.w * 1.25;
	text_rect.y = hex_rect.y + (hex_rect.h - text_rect.h) * .5;
	SDL_Rect global_rect = (SDL_Rect) {
		.x = hex_rect.x + hex_rect.w * .5,
		.y = hex_rect.y,
		.w = hex_rect.w * 1.25 + text_rect.w,
		.h = hex_rect.h 
	};
	
	//draw
	SDL_FillRect(ui->main_surf, &global_rect, HEX2MAP(ui->border_color));
	SDL_BlitSurface(ui->cell_tiles[ct], NULL, ui->main_surf, &hex_rect);
	SDL_BlitSurface(text_surf, NULL, ui->main_surf, &text_rect);
	SDL_FreeSurface(text_surf);

	global_rect.x -= hex_rect.w * .5;
	global_rect.w += hex_rect.w * .5;
	return global_rect;
}

int choice_menu(UI *ui, char *title, const int len, char **items) {
	ui_clear(ui);

	SDL_Rect item_rects[len];
	int total_height = (len + 1) * (ui->hex_rect.h + 2);
	int x = 3 * ui->hex_rect.w;
	int y = (ui->main_rect.h - total_height) / 2;
	hex_and_text(ui, EMPTY, -1, y, title);
	for (int i = 0; i < len; i++) {
		y += ui->hex_rect.h + 2;
		item_rects[i] = hex_and_text(ui, EMPTY + 1 + i % 6, x, y, items[i]);
	}
	SDL_UpdateWindowSurface(ui->main_win);

	SDL_Event e;
	SDL_Point mouse;
	SDL_Rect tmp_rect;
	while (1) {
		SDL_WaitEvent(&e);
		if (e.type == SDL_MOUSEBUTTONUP && e.button.button == 1) {
			mouse.x = e.button.x;
			mouse.y = e.button.y;
			for (int i = 0; i < len; i++) {
				if (SDL_PointInRect(&mouse, item_rects + i)) {
					ui_clear(ui);
					return i;
				}
			}
		} else if (e.type == SDL_MOUSEMOTION) {
			mouse.x = e.motion.x;
			mouse.y = e.motion.y;
			for (int i = 0; i < len; i++) {
				tmp_rect = item_rects[i];
				if (SDL_PointInRect(&mouse, &tmp_rect)) {
					SDL_BlitSurface(ui->selected_mask, NULL, ui->main_surf, &tmp_rect);
				} else {
					SDL_BlitSurface(ui->unselected_mask, NULL, ui->main_surf, &tmp_rect);
				}
				SDL_UpdateWindowSurface(ui->main_win);
			}
		}
	}
}

void ui_prompt_string(UI *ui, char *dst, const char *prompt) {
	SDL_StartTextInput();
	strcpy(dst, "");
	SDL_Event e;
	char *lengths;
	int len = 0;
	int done = 0;
	int redraw = 1;
	while (!done) {
		if (redraw) {
			ui_clear(ui);
			hex_and_text(ui, EMPTY, -1, (ui->main_rect.h * 0.65 - ui->hex_rect.h) / 2, prompt);
			hex_and_text(ui, EMPTY, -1, -1, dst);
			SDL_UpdateWindowSurface(ui->main_win);
			redraw = 0;
		}
		SDL_WaitEvent(&e);
		switch (e.type) {
		case SDL_TEXTEDITING:
			//TODO: see what this is about...
			break;
		case SDL_TEXTINPUT:
			strcat(dst, e.text.text);
			lengths = (char *) realloc(lengths, ++len * sizeof(char));
			lengths[len - 1] = strlen(e.text.text);
			SDL_Log("last_len: %d", lengths[len - 1]);
			redraw = 1;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_BACKSPACE) {
				if (len > 0) {
					SDL_Log("‘%s’ - ‘%.*s’", dst, lengths[len - 1], dst + strlen(dst) - lengths[len - 1]);
					char *new_dst = (char *) calloc(strlen(dst) - lengths[len - 1] + 1, sizeof(char));
					strncpy(new_dst, dst, strlen(dst) - lengths[len - 1]);
					strcpy(dst, new_dst);
					free(new_dst);
					lengths = (char *) realloc(lengths, --len * sizeof(char));
					SDL_Log("--> ‘%s’ (%ld)", dst, strlen(dst));
					redraw = 1;
				}
			} else if (e.key.keysym.sym == SDLK_RETURN) {
				done = strlen(dst) > 0;
			}
			break;
		case SDL_QUIT:
			done = 1;
			break;
		default:
			break;
		}
	}
	SDL_StopTextInput();
	free(lengths);
}

void print_status(UI *ui, Content ct, char *name) {
	//call hex_and_text
	static SDL_Rect last_rect = { .x = 0, .y = 0, .w = 0, .h = 0 };
	last_rect.x = ui->hex_rect.w / 2;
	last_rect.y = ui->hex_rect.h / 2;
	char text[64];
	sprintf(text, "Current player: %s", name);
	SDL_FillRect(ui->main_surf, &last_rect, HEX2MAP(ui->bg_color));
	last_rect = hex_and_text(ui, ct, last_rect.x, last_rect.y, text);

	//update display
	SDL_UpdateWindowSurface(ui->main_win);
}

void disp_msg(UI *ui, Content ct, char *msg) {
	//Wrong input
	if (ct == EMPTY) return;

	//Correct input
	ui_clear(ui);
	hex_and_text(ui, ct, -1, -1, msg);
	SDL_UpdateWindowSurface(ui->main_win);

	SDL_Event e;
	do {
		SDL_WaitEvent(&e);
	} while (e.type != SDL_KEYUP && (e.type != SDL_MOUSEBUTTONUP || e.button.button != 1));
	ui_clear(ui);
}

