#include <ncurses.h>
#include <string.h>

int main(int argc, char *argv[]){
	char msg[] = "A Big string switch i didn't care to type fully";
	int row, col;
	initscr();		// start curses mode
	getmaxyx(stdscr, row, col);
	start_color();	// start color functionality

	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	mvprintw(row/2, (col-strlen(msg))/2,msg);
	mvchgat(row/2, (col-strlen(msg))/2,-1, A_BLINK,1,NULL);
	refresh();
	getch();
	endwin();
	return 0;
}
