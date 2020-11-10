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
    int sd, n;
    char buf[BUFSIZ];
    struct sockaddr_in sin;

    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }

    //구조체 설정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect
    if(connect(sd, (struct sockaddr *)&sin, sizeof(sin))){
        perror("connect");
        exit(1);
    }

    if((n=recv(sd,buf,sizeof(buf), 0))==-1){
        perror("recv"); exit(1);
    }
    buf[n] = '\0';
    printf("%s\n",buf);
    

    printf("Input : ");
    gets(buf);
    
    if(send(sd,buf,sizeof(buf)+1, 0)==-1){
        perror("send"); exit(1);
    }

    if((n=recv(sd,buf,sizeof(buf), 0))==-1){
        perror("recv"); exit(1);
    }
    buf[n] = '\0';
    printf("Message > %s\n",buf);
    close(sd);
    return 0;
}