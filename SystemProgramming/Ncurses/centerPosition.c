#include <string.h>
#include <ncurses.h>

int main(){
    char msg[] = "Just a string";
    int row, col;

    initscr();  //start the curses mode
    getmaxyx(stdscr,row,col);     //get the number of rows and columns
    mvprintw(row/2,(col - strlen(msg))/2,msg);

    mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
    printw("Try resizing your window and then run this program again");
    refresh();
    getch();
    endwin();   //end the curses mode

    return 0;
}