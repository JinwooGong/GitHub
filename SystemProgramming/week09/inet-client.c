#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#define PORTNUM 9000 // 포트 번호

int main(void){
    char s_buf[256]; // 전송할 문자열을 담는 버퍼
    char r_buf[256]; // 받은 문자열을 담는 버퍼
    int sd;
    struct sockaddr_in sin; // 서버의 주소를 담는 구조체

    // 소켓 파일 기술자 생성
    if((sd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    
    // 소켓 구조체 설정
    memset((char *)&sin,'\0',sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM); // 호스트 바이트 주소를 네트워크 바이트 주소로 변환
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sd, (struct sockaddr*)&sin, sizeof(sin))==-1){
        perror("connect");
        exit(1);
    }
    while(1){
        printf("문자열입력(종료:q) : ");
        gets(s_buf);
        
        if(send(sd,s_buf,strlen(s_buf),0)==-1){
            perror("send");
            exit(1);
        }

        if(strcmp(s_buf,"q")==0){
            break;
        }

        printf("send success!!\n");
        if(recv(sd,r_buf, sizeof(r_buf),0)==-1){
            perror("recv");
            exit(1);
        }
        
        printf("%s\n",r_buf);
    }
    close(sd);
    return 0;
}