#Chinese Checkers Makefile

OBJ = main.o data_struct.o text_ui.o game.o
CC = gcc
LIBS = -lncurses
CFLAGS = -Wall
OUT = chinese-checkers.out

all: ${OBJ}
	${CC} ${CFLAGS} ${LIBS} -o ${OUT} ${OBJ}

data_struct.o:	data_struct.h
text_ui.o:	data_struct.h text_ui.h
game.o:		data_struct.h text_ui.h game.h
main.o:		data_struct.h text_ui.h game.h

.PHONY: clean
clean:
	-rm ${OUT} ${OBJ}

