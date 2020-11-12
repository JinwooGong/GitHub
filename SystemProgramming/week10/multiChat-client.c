#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define PORTNUM 9005    //9005번 포트번호 사용

int main(void)
{
    struct sockaddr_in ser;
    int sd;
    int n;
    char buf[BUFSIZ];
    pid_t pid;

    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){ perror("socket"); exit(1); }

    //구조체 설정
    memset((char *)&ser, '\0', sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser.sin_port = htons(PORTNUM);

    //connect
    if(connect(sd, (struct sockaddr *)&ser, sizeof(ser))){ perror("connect"); exit(1); }

    switch(pid=fork()){
        case -1:
            perror("fork");
            exit(1);
            break;

        case 0 : //자식 프로세스는 서버로부터 메세지를 받기만 함
            while(1){
                read(sd, buf, sizeof(buf));
                if(strncmp("<QUIT>", buf, strlen(buf))==0){
                    break;
                }

            }
            break;
        default:    //부모 프로세스는 서버에게 메세지를 보내기만 함
            while(1){
                //사용자로부터 입력을 받음
                n = read(0, buf, sizeof(buf));
                buf[n-1] = 0;

                //입력한 내용을 서버에게 전송
                write(sd, buf, sizeof(buf));
                if(strncmp("<QUIT>", buf, strlen(buf))==0){
                    break;
                }
                
            }
            break;
    }
    close(sd);
    return 0;
}