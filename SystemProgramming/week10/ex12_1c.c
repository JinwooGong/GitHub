#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//클라이언트도 서버와 같은 포트 번호를 사용해 소켓을 생성한다.
#define PORTNUM 9001

int main(void){
    int sd;
    char buf[256];
    struct sockaddr_in sin;
    
    memset((char *)&sin,'\0',sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); //접속할 서버의 주소를 지정한다.

    //소켓 생성
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    //서버와 접속
    if(connect(sd,(struct sockaddr*)&sin, sizeof(sin))==-1){
        perror("connect");
        exit(1);
    }
    //서버에 접속한 후 서버에서 보낸 메시지를 받아 40행에서 출력
    if(recv(sd, buf, sizeof(buf), 0) == -1){
        perror("recv");
        exit(1);
    }

    printf("*** From Server : %s\n",buf);

    //서버로 보낼 메시지를 복사한 후 서버로 메시지를 전송한다.
    strcpy(buf, "I want a HTTP Service.");
    if(send(sd, buf, sizeof(buf)+1, 0) == -1){
        perror("send");
        exit(1);
    }

    close(sd);
    return 0;
}