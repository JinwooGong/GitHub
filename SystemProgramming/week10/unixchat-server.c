#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_NAME "chat"

int main(void){
    char buf[256];
    struct sockaddr_un ser, cli;
    int sd, nsd, len, clen;
    int n;

    if((sd = socket(AF_UNIX, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }

    memset((char *)&ser, 0, sizeof(struct sockaddr_un));
    ser.sun_family=AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    if(bind(sd,(struct sockaddr *)&ser, len)){
        perror("bind");
        exit(1);
    }

    if(listen(sd, 5) < 0){
        perror("listen");
        exit(1);
    }

    if((nsd = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
        perror("accept");
        exit(1);
    }

    while(1){
        printf("Wating Message...\n");
        if(recv(nsd,buf,sizeof(buf), 0)== -1){
            perror("recv");
            exit(1);
        }
        printf("> You : %s\n",buf);
        if(strcmp(buf,"<QUIT>")==0)break;
        
        printf("Input text > ");
        gets(buf);
        printf("> Me : %s\n",buf);

        if(send(nsd, buf, sizeof(buf), 0) == -1){
            perror("send");
            exit(1);
        }
        if(strcmp(buf,"<QUIT>")==0) break;
       
    }
    close(nsd);
    close(sd);

    return 0;
}