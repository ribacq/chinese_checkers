# Chinese Checkers Makefile
#
# Written by Quentin RIBAC, 2017
# This is free software.
# See LICENSE file for legal information

CC = gcc
CFLAGS = -Wall
OUT = chinese-checkers.out
SRC = data_struct.c game.c main.c

TUI = text_ui.c
TUI_LIBS = -lncurses

GUI = gui.c
GUI_LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

tui:
	$(CC) $(CFLAGS) $(TUI_LIBS) -o $(OUT) $(SRC) text_ui.c

gui:
	$(CC) $(CFLAGS) -D_GUI $(GUI_LIBS) -o $(OUT) $(SRC) gui.c

clean:
	-rm $(OUT)

.PHONY: tui gui clean

