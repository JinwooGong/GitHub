#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h> // AF_INET 외부 네트워크 도메인
#include<sys/types.h>
#include<sys/socket.h>

#define MAXLINE 511
#define PORTNUM 9005

int main(void)
{
    struct sockaddr_in ser;
    int sd;
    int iRet;
    char cBuff[BUFSIZ];
    pid_t pid;

    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket"); exit(1);
    }

    memset((char *)&ser, '\0', sizeof(ser));
    ser.sin_family = AF_INET;

    ser.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser.sin_port = htons(PORTNUM);

    if(connect(sd, (struct sockaddr *)&ser, sizeof(ser))){
        perror("connect");
        exit(1);
    }


    pid = fork();

    if(pid > 0)
    {
        close(0);
        while(1)
        {
            read(sd, cBuff, sizeof(cBuff));
            printf("[server]: [%s]\n", cBuff);

            if(0 == strncmp("exit", cBuff, strlen(cBuff))){

            break;
            }
        }
    }
    else{
        close(1);
        while(1){
            iRet = read(0, cBuff, sizeof(cBuff));
            cBuff[iRet-1] = 0;
            write(sd, cBuff, sizeof(cBuff));
        }
    }

    close(sd);
    return 0;
}