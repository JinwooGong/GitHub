#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define PORTNUM 9010

int main(void){
    char buf[256], tmp[256];
    struct sockaddr_in sin, cli;
    int sd, ns, clen=sizeof(cli);
    int i;

    //socket
    if((sd=socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }

    //구조체 설정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    //bind
    if(bind(sd,(struct sockaddr *)&sin, sizeof(sin))){
        perror("bind");
        exit(1);
    }

    //listen
    if(listen(sd,5) < 0){
        perror("listen");
        exit(1);
    }

    while(1){
        if((ns = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
            perror("accept");
            exit(1);
        }
        printf("** Accept Client\n");
        
        strcpy(buf,"<Available Services>\n1. Echo server <ECHO>\n2. Get server info <SINFO>\n3. Get server time <STIME>\n");
        switch(fork()){
            case 0 : /* child */
                close(sd);
                if(send(ns,buf,sizeof(buf)+1,0)==-1){
                    perror("send");
                    exit(1);
                }
                if(recv(ns,buf, sizeof(buf), 0)==-1){
                    perror("recv");
                    exit(1);
                }
                
                if(strncmp(buf,"<ECHO>",5)==0){
                    printf("** From Client : %s\n",buf);
                    
                    if(send(ns,buf,sizeof(buf)+1, 0)==-1){
                        perror("send");
                        exit(1);
                    }
                }
                break;
        }
        close(ns);
    }
    close(sd);
    return 0;
}