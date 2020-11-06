#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORTNUM 9090

int main(int argc, char *argv[]){
    char buf[256];
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);

    
    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }

    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(argv[2]));
    sin.sin_addr.s_addr = inet_addr(argv[1]);

    //bind
    if(bind(sd,(struct sockaddr *)&sin, sizeof(sin))){
        perror("bind");
        exit(1);
    }

    //listen
    if(listen(sd, 5) < 0) {
        perror("listen");
        exit(1);
    }
    
    //accept
    if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen))==-1){
        perror("accept");
        exit(1);
    }
    
    while(1){
        printf("Wating Message...\n");
        if(recv(ns,buf,sizeof(buf), 0)== -1){
            perror("recv");
            exit(1);
        }
        printf("> You : %s\n",buf);
        if(strcmp(buf,"<QUIT>")==0)break;
        
        printf("Input text > ");
        gets(buf);
        printf("> Me : %s\n",buf);

        if(send(ns, buf, sizeof(buf), 0) == -1){
            perror("send");
            exit(1);
        }
        if(strcmp(buf,"<QUIT>")==0) break;
    }
    
    close(ns);
    close(sd);
    return 0;
}