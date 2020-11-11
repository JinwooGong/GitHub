#include <ncurses.h>
#include <string.h>

int main(void){
    char msg[] = "Enter a string : ";
    char str[80];
    int row, col;

    initscr();

    getmaxyx(stdscr,row,col);
    mvprintw(row/2,(col-strlen(msg))/2,msg);
    getstr(str);

    mvprintw(LINES-2,0, "You Entered : %s",str);
    refresh();
    getch();
    endwin();
    return 0;
}