#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define PORTNUM 9010 //포트 번호

int main(void){
    char buf[BUFSIZ], tmp[BUFSIZ];
    struct sockaddr_in sin, cli;
    struct hostent *hent;
    
    int sd, ns, clen=sizeof(cli);
    int i,j, n;

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

    while(1){ //동시 동작 서버
        if((ns = accept(sd,(struct sockaddr *)&cli, &clen))==-1){
            perror("accept"); exit(1);
        }
        //menu
        strcpy(buf,"<Available Services>\n1. Echo server <ECHO>\n2. Get server info <SINFO>\n3. Get server time <STIME>\n");

        switch(fork()){
            case 0 : // 자식 프로세스
                close(sd);
                //menu send
                if(send(ns,buf,strlen(buf)+1,0)==-1){
                    perror("send"); exit(1);
                }

                if(recv(ns,buf,strlen(buf),0)==-1){
                    perror("recv"); exit(1);
                }
                printf("**From Client : %s\n",buf);

                if(strncmp(buf,"<ECHO>",6)==0){
                    for(i=0, j=6;j<=strlen(buf);i++,j++){
                        tmp[i] = buf[j];
                    }
                    
                    if(send(ns,tmp,strlen(tmp)+1,0)==-1){
                        perror("send"); exit(1);
                    }
                }
                else if(strcmp(buf,"<STIME>")==0){
                    dup2(ns,STDIN_FILENO);
                    dup2(ns,STDOUT_FILENO);
                    close(ns);
                    execl("./time","time",(char *)0);
                }
                else if(strcmp(buf,"<SINFO>")==0){
                    sethostent(0);

                    while((hent = gethostent())!=NULL){

                        sprintf(buf,"Host : %s, IP : %s, PortNum : %d\n", hent->h_name, inet_ntoa(*(struct in_addr*)hent->h_addr_list[0]),PORTNUM);
                        if(send(ns,buf,sizeof(buf)+1,0)==-1){
                            perror("send");
                            exit(1);
                        }
                    }
                    endhostent();
                }
                else if(strcmp(buf,"<QUIT>")==0){
                    close(ns);
                }
                
                sleep(1);
                break;
        }
        close(ns);
    }
    return 0;
}