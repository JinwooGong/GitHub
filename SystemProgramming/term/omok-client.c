#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ncurses.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <fcntl.h>

//border의 크기 (11x11)
#define BORDER_X 11
#define BORDER_Y 11

#define PORTNUM 9010

char *pObjs[] = {"○", "●"};

struct termios attribs;

struct cursor {
	   int x;
	   int y;
};

/*for socket connection*/
char messageT[BUFSIZ]; //착수가 끝나면 좌표값과 순서를 저장하는 변수
int sock;
pthread_t thread_id;
char name[2][100];

struct omokgame {
    struct cursor cur;
    int turn;
    int board[BORDER_X][BORDER_Y];
    int conti;
};

static struct omokgame game;
void drawBoard();
void drawChar(int x, int y, char *str);
void input(int x, int y, int c);
void spaceKey(int kVal);
void moveCursor(int kVal);
void *recv_manager(void *socket_desc);
void key_manager();
int endGame(int c);
int getBoardState(int x, int y);

int main() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    struct sockaddr_in server;
    char message[BUFSIZ] , server_reply[BUFSIZ];

    //keypad(stdscr,TRUE);
    //Create socket
    if((sock = socket(AF_INET , SOCK_STREAM , 0))==-1){
        perror("socket"); exit(1);
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORTNUM);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) == -1){
        perror("connect"); exit(1);
    }
    puts("Connected\n");

    // turn message
    int read_size = recv(sock , message , BUFSIZ , 0);
    message[read_size] ='\n';

    game.turn = atoi(message);
    puts("");
    puts("Input your ID >>");
    scanf("%s",name[game.turn]);
    printf("Wellcome %s\n",name[game.turn]);
    if(game.turn==0) puts("First turn");
    else puts("Second turn");

    sleep(2);


    if( pthread_create( &thread_id , NULL ,  recv_manager , (void*) &sock) == -1){
        perror("pthread_create"); exit(1);
    }

    //locale 설정
    char *lc = getenv("LC_CTYPE");
    if(lc != NULL)
            setlocale(LC_CTYPE, lc);
    else if((lc = getenv("LC_ALL")))
            setlocale(LC_CTYPE, lc);
    else
            setlocale(LC_CTYPE, "");


    initscr(); // ncurses 시작
    keypad(stdscr,TRUE);
    drawBoard();
    game.conti = 1;

    noecho();

    while(1) {
        if(game.conti == 0){
            sleep(3);
            break;
        }
        key_manager(); //key_manager()함수 호출
    }
    endwin(); //ncurses 종료
}

void key_manager() {
    //키를 입력받고 각 함수에 인자로 전달함
    int keyVal = getch();
    moveCursor(keyVal); //커서 이동 함수
    spaceKey(keyVal);
}

void drawChar(int x, int y, char *str) {
    move(x, y*2);
    printw(str);
    refresh();
}


void drawBoard(){
    int x, y;

    for( x = 0; x < BORDER_X ; x++){
        drawChar(x,0, "├");
        drawChar(x, BORDER_Y - 1,"┤");
        game.board[x][0] = -1;
        game.board[x][BORDER_Y - 1] = -1;
    }

    for( y = 0; y < BORDER_Y ; y++){
        drawChar(0, y, "┬");
        drawChar(BORDER_X - 1, y, "┴");
        game.board[0][y] = -1;
        game.board[BORDER_X - 1][y] = -1;
    }

    for(  x = 1; x < BORDER_X - 1 ; x++){
        for( y = 1; y < BORDER_Y - 1 ; y++){
                drawChar(x,y, "┼");
                game.board[x][y] = -1;
        }
    }

	drawChar(0,0, "┌");
	drawChar(BORDER_X - 1, 0, "└");
	drawChar(0, BORDER_Y - 1, "┐");
	drawChar(BORDER_X - 1, BORDER_Y - 1, "┘");
    mvprintw(BORDER_X+1, 0,"You : %s",name[game.turn]);
    if(game.turn == 0) mvprintw(BORDER_X+2,0,"Your Turn");
        else mvprintw(BORDER_X+2,0,"Waiting Turn");
	move(0,0);
}

void moveCursor(int key) {
    switch(key) {
        case KEY_LEFT : //왼쪽으로 이동
            game.cur.y--;
            if(game.cur.y <0) game.cur.y = 0;
            move(game.cur.x,game.cur.y*2);
            break;
        case KEY_RIGHT : //오른쪽으로 이동
            game.cur.y++;
            if(game.cur.y >BORDER_Y -1) game.cur.y = BORDER_Y -1;
            move(game.cur.x,game.cur.y*2);
            break;
        case KEY_UP: //위로 이동
            game.cur.x--;
            if(game.cur.x <0) game.cur.x = 0;
            move(game.cur.x,game.cur.y*2);
            break;
        case KEY_DOWN: //아래로 이동
            game.cur.x++;
            if(game.cur.x > BORDER_X -1) game.cur.x = BORDER_X -1;
            move(game.cur.x,game.cur.y*2);
            break;
    }
}

void input(int x, int y, int c) {
    if(game.board[x][y] == -1 && x != 0 && x != BORDER_X-1 && y != 0 && y != BORDER_Y-1) {
        drawChar(x, y, pObjs[c]); // 바둑돌을 그림
        game.board[x][y] = c; //2차원 배열에 channel 표시

        if(endGame(c)) //게임이 끝나면 game.conti에 0 저장
            game.conti = 0;
    }
}

void spaceKey(int kVal) {
    int c = game.turn % 2;
    switch(kVal) {
        case ' ' : //SpaceBar 클릭
        //좌표값과 turn값을 messageT에 저장
        sprintf(messageT, "%d%d%d", game.cur.x,game.cur.y, c);
        send(sock , messageT , strlen(messageT) , 0); //서버에게 메세지를 보냄
        break;
    }
}

void *recv_manager(void *socket_desc)
{
    //Get the socket descriptor
    int sock2 = *(int*)socket_desc;
    int read_size;
    char *message , client_message[BUFSIZ];

    char temp[BUFSIZ];
    int x, y, c;
    int xyc[3];

    //Receive a message from client
    while( (read_size = recv(sock , client_message , BUFSIZ , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';

        //Send the message back to client
        memcpy(temp, client_message, sizeof(client_message));      //받은 메세지를 임시배열로 복사

        x = atoi(temp)/100;
        y = atoi(&temp[1])/10%100;
        c = atoi(&temp[2])%10;

        input(x,y,c);
        mvprintw(BORDER_X+2,0,"                  ");
        if(c!=game.turn) mvprintw(BORDER_X+2,0,"Your Turn");
        else mvprintw(BORDER_X+2,0,"Waiting Turn");
        refresh();
        //client_message 메모리 초기화
        memset(client_message, 0, BUFSIZ);
    }
    if(read_size == -1){
        perror("recv"); exit(1);
    }
    return 0;
}

int getBoardState(int x, int y) {
    if(x>=0 && x<BORDER_X && y>=0 && y<BORDER_Y) {
            return game.board[x][y];
    }
    return 2;
}

int endGame(int c) {

    int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

    int x, y, color, d;

    for(x = 1; x<BORDER_X; x++)
        for(y = 1; y<BORDER_Y; y++) {
            color = getBoardState(x,y);
            if(color == c) {
                for(d = 0; d < 8; d++) {
                    int i=1;
                    for(i = 1; i< 5; i++) {
                        int tx = x+(dx[d] *i);
                        int ty = y+(dy[d] *i);

                        if(getBoardState(tx,ty) != c)
                            break;
                    }
                    if(i==5) {
                        move(13,0);
                        if(c)   mvprintw(BORDER_X+5,0,"********** Second win! ***********");
                        else    mvprintw(BORDER_X+5,0,"********** First win! ***********");
                        refresh();
                        return 1;
                    }
                }
            }
        }
    return 0;
}