#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "practice_socket"
#define SIZE 1024

int main(void){
    int sd, len;
    char s_buf[SIZE];   // send buffer
    char r_buf[SIZE];   // recv buffer
    int n;

    struct sockaddr_un ser;
    //socket
    if((sd=socket(AF_UNIX, SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    //구조체 설정
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
        gets(s_buf); //파일의 정대 경로 입력
        
        //서버에게 정대경로 send
        if(send(sd, s_buf, SIZE, 0) == -1){
            perror("send");
            exit(1);
        }
        
        //서버로부터 파일 내용 rcev
        if((n=recv(sd, r_buf,SIZE,0))==-1){
            perror("recv");
            exit(1);
        }
        
        while(n>=SIZE){ //byte초과
            //출력후 다시 recv
            r_buf[n]='\0';
            printf("%s",r_buf);
            n=recv(sd, r_buf,SIZE,0);
        }

        r_buf[n]='\0'; //남은 buffer 출력 및 byte가 초과되지 않은 내용 출력
        printf("%s",r_buf);

    }
    
    close(sd);
    return 0;
}