#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>

void move_arrow_key(char chr, int *x, int *y, int x_b, int y_b);
void gotoxy(int x, int y);
void draw_pan(int c, int r);
void game_control(void);
void display_stone(int matrix[][20][20]);
int game_end(int matrix[][20][20]);

int main(void)
{
        system("cls");
        game_control();
        return 0;
}

//번갈아가면서 바둑돌을 놓는 함수
void game_control(){
    int x=1,y=1,other=0;
    int matrix[2][20][2] = {0};
    char key;
    char *dol[2] = {"○", "●"};

    while(1){
        gotoxy(1,1);
        draw_pan(18,18);
    }
}