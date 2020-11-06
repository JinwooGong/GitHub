#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PORTNUM 9005

int main(void){
    char buf[256];
    struct sockaddr_in sin, cli;
    int sd, clen = sizeof(cli);

    struct tm *tm;
    time_t t;

    if((sd = socket(AF_INET, SOCK_DGRAM , 0))==-1){
        perror("socket");
        exit(1);
    }

    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    //bind
    if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))){
        perror("bind");
        exit(1);
    }

    while(1){
        if((recvfrom(sd, buf, 255, 0, (struct sockaddr *)&cli, &clen)) == -1){
            perror("recvfrom");
            exit(1);
        }
        printf("*** From Client : %s\n", buf);

        time(&t);
        tm = localtime(&t);
        strftime(buf, sizeof(buf), "%G년, %m월 %d일 %H:%M:%S", tm);
        if((sendto(sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&cli, sizeof(cli)))==-1){
            perror("sendto");
            exit(1);
        }
    }
    return 0;
}