#Chinese Checkers Makefile

OBJ = main.o data_struct.o text_ui.o
CC = gcc
LIBS = -lncurses
CFLAGS = -Wall

chinese-checkers: ${OBJ}
	${CC} ${CFLAGS} ${LIBS} -o chinese-checkers ${OBJ}

main.o: data_struct.h text_ui.h
data_struct.o: data_struct.h
text_ui.o: text_ui.h data_struct.h

.PHONY: clean
clean:
	-rm chinese-checkers ${OBJ}

