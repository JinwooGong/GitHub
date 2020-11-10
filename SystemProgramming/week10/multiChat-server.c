#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

 #define PORTNUM 9005

int main(void){
    char buf[BUFSIZ];
    struct sockaddr_in sin,cli;
    int sd, ns, clen = sizeof(cli);

    pid_t pid;

    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket"); exit(1);
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
        //accept
        if((ns = accept(sd,(struct sockaddr *)&cli, &clen))==-1){
            perror("accept"); exit(1);
        }
    }

    return 0;
}