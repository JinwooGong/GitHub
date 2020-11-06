#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SOCKET_NAME "chat"

int main(void){
    char buf[256];
    struct sockaddr_un ser;
    int sd, len, flag;

    memeset((char *)&ser, '\0', sizeof(ser));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    if((sd = socket(AF_UNIX,SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    if(connect(sd, (struct sockaddr*)&ser, len)){
        perror("connect");
        exit(1);
    }
    flag = fcntl(nsd, F_GETFL, 0);
    fcntl(nsd, F_SETFL, flag | O_NONBLOCK);
    while(1){
        printf("Input Text: ");
        gets(buf);
        if(recv(nsd, buf, strlen(buf), 0) == -1){
            printf("YOU : %s\n",buf);
        }
        if(send(nsd, buf, strlen(buf)+1, 0) == -1){
            printf("ME : %s\n",buf);
        }
        if(strcmp(buf,"<QUIT>")==0){
            break;
        }
    }
    close(sd);
    return 0;
}