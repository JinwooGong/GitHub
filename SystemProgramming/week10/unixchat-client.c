#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_NAME "chat" //chat이름의 소켓 사용

int main(void){
    char buf[256];
    struct sockaddr_un ser;
    int sd, len, flag;
    int n;

    //socekt
    if((sd = socket(AF_UNIX, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }

    //구조체 설정
    memset((char *)&ser,'\0', sizeof(struct sockaddr_un));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    //connect
    if(connect(sd, (struct sockaddr *)&ser, len)<0){
        perror("connect");
        exit(1);
    }

    while(1){
        //서버에게 메세지를 보냄
        printf("Input text > ");
        gets(buf);
        printf("> Me : %s\n",buf);
        if(send(sd, buf, sizeof(buf), 0) == -1){
            perror("send");
            exit(1);
        }

        //보낸 매세지가 <QUIT>이면 종료
        if(strcmp(buf,"<QUIT>")==0) break;

        //서버로 부터 메세지가 올때 까지 기다림
        printf("Wating Message...\n");
        if(recv(sd, buf, sizeof(buf), 0) == -1){
            perror("recv");
            exit(1);
        }
        //받은 메세지를 화면에 출력
        printf("> You : %s\n",buf);
        if(strcmp(buf,"<QUIT>")==0) break;
    }
    close(sd);
    return 0;
}