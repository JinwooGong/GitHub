#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define PORTNUM 9005

int sd_arr[5] = {0,0,0,0,0};

int main(void)
{
    
    int iRet;
    struct sockaddr_in sin ,cli;
    char buf[BUFSIZ];
    int sd, ns, clen = sizeof(cli);
    int n=0, i;
    

    pid_t pid;

    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    } 


    // sin 구조체에 정보 입력
    memset((char *)&sin, '\0', sizeof(sin));            //구조체 초기화
    sin.sin_family = AF_INET;               
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");   
    sin.sin_port = htons(PORTNUM);

    if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))==-1){
        perror("bind"); exit(1);
    }

    if(listen(sd, 5) < 0){
        perror("listen"); exit(1);
    }
    while(1) 
    {
        printf("Accepting...\n");
        if((ns = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
            perror("accept"); exit(1);
        }

        printf("Client  Accept\n");
        printf("Client IP :%s\n", inet_ntoa(cli.sin_addr));

        write(ns, "connected", sizeof("connected"));
        switch(pid = fork()){
            case -1:
                perror("fork");
                exit(1);
                break;
            case 0:
                close(sd);
                while(1){
                    read(ns, buf, sizeof(buf));
                    printf("[%d]: %s\n",ns, buf);

                    write(ns,buf,sizeof(buf));
                    if(strncmp(buf, "exit", strlen(buf))==0){
                        printf("Exited %d\n",ns);
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