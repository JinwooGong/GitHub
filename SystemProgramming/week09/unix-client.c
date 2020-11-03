#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "practice_socket"
#define SIZE 1024

int main(void){
    int sd, len;
    char s_buf[SIZE];
    char r_buf[SIZE];
    char file_path[SIZE];
    int n;

    struct sockaddr_un ser;

    if((sd=socket(AF_UNIX, SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    
    memset((char *)&ser,'\0', sizeof(ser));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path,SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);
    
    
    if(connect(sd, (struct sockaddr *)&ser, len)){
        perror("connect");
        exit(1);
    }
    while(1){
        printf("\n\nInput File Path : ");
        gets(s_buf);
        //strcpy(s_buf,"/home/sp20135103/git/SystemProgramming/test.txt");
        if(send(sd, s_buf, SIZE, 0) == -1){
            perror("send");
            exit(1);
        }

        if((n=recv(sd, r_buf,SIZE,0))==-1){
            perror("recv");
            exit(1);
        }
        
        while(n>=SIZE){
            r_buf[n]='\0';
            printf("%s",r_buf);
            n=recv(sd, r_buf,SIZE,0);
        }
        r_buf[n]='\0';
        printf("%s",r_buf);

    }
    
    close(sd);
    return 0;
}