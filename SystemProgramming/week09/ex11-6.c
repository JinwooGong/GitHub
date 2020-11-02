#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_NAME "hbsocket"

int main(void){
    char buf[256];
    struct sockaddr_un ser, cli;
    int sd, nsd, len, clen;

    if((sd = socket(AF_UNIX,SOCK_STREAM,0))==-1){
        perror("socekt");
        exit(1);
    }

    memset((char*)&ser,0,sizeof(struct sockaddr_un));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path,SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);
    printf("ser.sun_family : %d, ser.sun_path : %d\n",sizeof(ser.sun_family), strlen(ser.sun_path));
    
    if(bind(sd, (struct sockaddr *)&ser,len)){
        perror("bind");
        exit(1);
    }

    if(listen(sd,5) < 0){
        perror("listen");
        exit(1);
    }

    printf("Waiting ...\n");
    if((nsd = accept(sd,(struct sockaddr *)&cli, &clen))==-1){
        perror("accept");
        exit(1);
    }

    return 0;
}