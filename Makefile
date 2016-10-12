#Chinese Checkers Makefile

OBJ = main.o data_struct.o text_ui.o game.o
CC = gcc
LIBS = -lncurses
CFLAGS = -Wall
OUT = chinese-checkers.exe

all: ${OBJ}
	${CC} ${CFLAGS} ${LIBS} -o ${OUT} ${OBJ}

main.o: data_struct.h text_ui.h game.h
data_struct.o: data_struct.h
text_ui.o: text_ui.h data_struct.h
game.o: text_ui.c data_struct.h

.PHONY: clean
clean:
	-rm ${OUT} ${OBJ}

