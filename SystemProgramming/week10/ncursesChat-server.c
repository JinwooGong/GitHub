#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int main(int argc, char *argv[]){
    char buf[256];
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);
    int row,col ,r=0,c=0;

    //3개의 인자가 들어오지 않으면 비정상 종료
    if(argc!=3){ perror("argc"); exit(1); }

    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){ perror("socket"); exit(1); }

    //구조체 설정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(argv[2]));
    sin.sin_addr.s_addr = inet_addr(argv[1]);

    //bind
    if(bind(sd,(struct sockaddr *)&sin, sizeof(sin))){ perror("bind"); exit(1); }

    //listen
    if(listen(sd, 5) < 0) { perror("listen"); exit(1); }
    
    //accept
    if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen))==-1){ perror("accept"); exit(1); }

    initscr();
    getmaxyx(stdscr,row,col);
    mvprintw(r++,0,"Client Message...\n");
    while(1){
        //클라이언트로부터 메세지가 올때 까지 기다림
        
        if(recv(ns,buf,sizeof(buf), 0)== -1){ perror("recv"); exit(1); }

        //클라이언트에게 받은 메세지 출력
        mvprintw(r++,0,"> You : %s\n",buf);
        refresh();
        //받은 메세지가 <QUIT>이면 반복문 종료
        if(strcmp(buf,"<QUIT>")==0)break;
        
        //클라이언트에게 메세지 전송
        mvprintw(row-2,0,"Input text > ");
        refresh();
        getstr(buf);
        mvprintw(row-1,0,"> Me : %s\n",buf);
        refresh();

        if(send(ns, buf, sizeof(buf), 0) == -1){ perror("send"); exit(1); }

        if(r==row-3) clear();
        //입력한 메세지가 <QUIT>이면 반복문 종료
        if(strcmp(buf,"<QUIT>")==0) break;
    }
    
    close(ns);
    close(sd);
    endwin();
    return 0;
}