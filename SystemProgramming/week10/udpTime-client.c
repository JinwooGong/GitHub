#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORTNUM 9005 //9005번 포트번호 사용

int main(void){
    char buf[256];
    struct sockaddr_in sin;
    int sd, n;

    //socket
    if((sd = socket(AF_INET, SOCK_DGRAM ,0))==-1){
        perror("socket");
        exit(1);
    }

    //구조체 설정
    memset((char *)&sin,'\0',sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    //서버에게 요청을 보냄
    strcpy(buf,"Client");
    if(sendto(sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&sin, sizeof(sin))==-1){
        perror("sendto");
        exit(1);
    }
    //요청받은 시간을 화면에 출력
    n = recvfrom(sd ,buf, 255, 0, NULL, NULL);
    buf[n] = '\0';
    printf("** From Server : %s\n",buf);
}