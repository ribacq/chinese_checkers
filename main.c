#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "data_struct.h"

int main(int argc, char* argv[])
{
	//Hello world!
	initscr();
	printw("Hello world!\n");
	refresh();

	//Board test
	int n=4;
	Content* b = init_board(n);
	printw("\n");
	print_board(b, n);
	
	//End
	printw("\nPress a key to exit.");
	refresh();
	getch();
	endwin();
	return 0;
}
