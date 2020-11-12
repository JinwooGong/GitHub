#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_NAME "chat"

int main(void){
    char buf[256];
    struct sockaddr_un ser, cli;
    int sd, nsd, len, clen;
    int n;

    //socekt
    if((sd = socket(AF_UNIX, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }

    //구조체 설정
    memset((char *)&ser, '\0', sizeof(struct sockaddr_un));
    ser.sun_family=AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    //bind
    if(bind(sd,(struct sockaddr *)&ser, len)){
        perror("bind");
        exit(1);
    }

    //listen
    if(listen(sd, 5) < 0){
        perror("listen");
        exit(1);
    }

    //accept
    if((nsd = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
        perror("accept");
        exit(1);
    }

    while(1){
        //클라이언트로부터 메세지가 올때 까지 기다림
        printf("Wating Message...\n");
        if(recv(nsd,buf,sizeof(buf), 0)== -1){
            perror("recv");
            exit(1);
        }

        //메세지가 오면 화면에 출력
        printf("> You : %s\n",buf);

        //<QUIT>이면 반복문 종료
        if(strcmp(buf,"<QUIT>")==0)break;
        
        //클라이언트에게 메세지를 전송
        printf("Input text > ");
        gets(buf);
        printf("> Me : %s\n",buf);

        if(send(nsd, buf, sizeof(buf), 0) == -1){
            perror("send");
            exit(1);
        }
        //전송한 메세지가 <QUIT>이면 반복문 종료
        if(strcmp(buf,"<QUIT>")==0) break;
       
    }
    close(nsd);
    close(sd);

    return 0;
}