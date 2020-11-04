#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCKET_NAME "practice_socket"
#define SIZE 1024

int main(void){
    int sd, len, n;
    char s_buf[SIZE]; //send 메시지 버퍼
    char r_buf[SIZE]; //recv 메시지 버퍼

    struct sockaddr_un ser;

    //socket 생성
    if((sd=socket(AF_UNIX, SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    
    //sockaddr_un ser 구조체 설정
    memset((char *)&ser,'\0', sizeof(ser));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path,SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);
    
    //connect
    if(connect(sd, (struct sockaddr *)&ser, len)){
        perror("connect");
        exit(1);
    }
    while(1){
        printf("\n\nInput File Path : ");
        gets(s_buf);//절대경로 입력
        // "/home/sp20135103/git/tutorial.txt"

        //send
        if(send(sd, s_buf, SIZE, 0) == -1){
            perror("send");
            exit(1);
        }
        //recv
        if((n=recv(sd, r_buf,SIZE,0))==-1){
            perror("recv");
            exit(1);
        }
        
        while(n>=SIZE){ //서버로 부터 받은 버퍼 사이즈가 1024바이트가 넘으면
            r_buf[n]='\0';
            printf("%s",r_buf);
            n=recv(sd, r_buf,SIZE,0); //1024 바이트 이후의 버퍼를 받아옴
        }
        r_buf[n]='\0';
        printf("%s",r_buf); //마지막 버퍼 출력

    }
    
    close(sd);
    return 0;
}