#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define PORTNUM 9005 //9005번 포트 사용

int main(void)
{
    struct sockaddr_in sin ,cli;
    char buf[BUFSIZ];
    int sd, ns, clen = sizeof(cli);
    int n;
    

    pid_t pid;

    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    } 


    // sin 구조체에 정보 입력
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;               
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");   
    sin.sin_port = htons(PORTNUM);

    //bind
    if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))==-1){ perror("bind"); exit(1); }
    //listen
    if(listen(sd, 5) < 0){ perror("listen"); exit(1); }

    while(1) //동시 동작 서버
    {
        //클라이언트 accepting
        printf("Accepting...\n");
        if((ns = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
            perror("accept"); exit(1);
        }

        //클라이언트가 접속하면 정보를 화면에 출력
        printf("Client  Accept\n");
        printf("Client IP :%s\n", inet_ntoa(cli.sin_addr));

        //자식프로세스로 통신을 하기위해 fork()함수 호출
        switch(pid = fork()){
            case -1:
                perror("fork");
                exit(1);
                break;
            case 0: //자식 프로세스
                close(sd);
                while(1){
                    //클라이언트로부터 읽어온 메세지를 화면에 출력
                    read(ns, buf, sizeof(buf));
                    printf("[%d]: %s\n",ns, buf);

                    write(ns,buf,sizeof(buf));
                    if(strncmp(buf, "<QUIT>", strlen(buf))==0){
                        printf("[%d] has left\n",ns);
                        close(ns);
                        break;
                    }
                }
                break;
        }
    }
    close(ns);
    close(sd);
    return 0;
}