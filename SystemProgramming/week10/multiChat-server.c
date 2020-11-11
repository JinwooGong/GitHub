#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h> //AF_INET 외부 내트워크 도메인
#include<sys/types.h>
#include<sys/socket.h>

#define MAXLINE 511
#define PORTNUM 9005

int main(void)
{
    
    int iRet;
    struct sockaddr_in sin ,cli;
    char cBuff[BUFSIZ];
    int sd, ns, clen = sizeof(cli);

    pid_t pid;

    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    } 


    // stAddr구조체에 socket연결을 위한 필수 정보 입력  setting
    memset((char *)&sin, '\0', sizeof(sin));            //구조체 비우기(0으로 채우기)
    sin.sin_family = AF_INET;               //#define AF_INET 2 /* IP protocol family. */
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");    //IP와 PORT값은 헤더파일에 정의되어 있다.
    sin.sin_port = htons(PORTNUM);

    if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))==-1){
        perror("bind"); exit(1);
    }

    if(listen(sd, 5) < 0){
        perror("listen"); exit(1);
    }
    while(1) 
    {
        if((ns = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
            perror("accept"); exit(1);
        }

        
        /////////////////////////////멀티 프로세스///////////////////////////////////////////
        pid = fork();

        if(pid ==  0)  //자식 프로세스이면 while문 break
        {
            break;
        }
    }
    close(sd);

    printf("Incoming Client \n");
    printf("Client IP :%s\n", inet_ntoa(cli.sin_addr));
    printf("Client Port : %d\n", ntohs(cli.sin_port));  

    write(IN_CLASSB_NSHIFT, "Welcome :)", sizeof("Welcome :)"));

    while(1)
    {
        read(ns, cBuff, sizeof(cBuff));
        printf("[client]: [%s]\n", cBuff);

        write(ns, cBuff, sizeof(cBuff));
        if(0 == strncmp("exit", cBuff, strlen(cBuff) ) )
        break;
    }
    close(ns);
    return 0;
}