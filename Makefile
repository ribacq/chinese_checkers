# Chinese Checkers Makefile
#
# Written by Quentin RIBAC, 2017
# This is free software.
# See LICENSE file for legal information

CC = gcc
CFLAGS = -Wall
OUT = chinese-checkers.out
OBJ = main.o data_struct.o game.o

TUI = text_ui.o
TUI_LIBS = -lncurses

GUI = gui.o
GUI_LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
GUI_CFLAGS = -D _GUI

tui: ${TUI} ${OBJ}
	${CC} ${CFLAGS} ${TUI_LIBS} -o ${OUT} ${OBJ} ${TUI}

gui: ${GUI} ${OBJ}
	${CC} ${CFLAGS} ${GUI_CFLAGS} ${GUI_LIBS} -o ${OUT} ${OBJ} ${GUI}

data_struct.o: data_struct.h
game.o:        data_struct.h game.h
main.o:        data_struct.h game.h

clean:
	-rm *.o

fclean: clean
	-rm ${OUT}

.PHONY: all gui clean fclean

