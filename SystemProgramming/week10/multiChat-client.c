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
    struct sockaddr_in sin;
    int sd, n;

    pid_t pid;

    //socket
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket"); exit(1);
    }

    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sd,(struct sockaddr *)&sin, sizeof(sin))){
        perror("connect"); exit(1);
    }

    switch(pid=fork()){
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            while(1){
                fgets(buf,sizeof(buf),stdin);
                n=strlen(buf);
                write(sd,buf,sizeof(buf));
                if(strncmp(buf,"exit",4)==0){
                    printf("exit\n");
                    break;
                }
            }
            break;
        default:
            while(1){
                if(n = read(sd,buf,sizeof(buf))<0){
                    perror("read"); exit(1);
                }
                printf("** From Server : %s\n",buf);
                if(strncmp(buf,"exit",4)==0){
                    break;
                }
            }
            break;
    }
    close(sd);
    return 0;
}