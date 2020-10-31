#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORTNUM 9000

int main(void){
    char buf[256]; // 클라이언트로부터 받은 문자열을 저장할 buf
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);
    char *msg = "Server recevied the message";
    int n;

    // 소켓 파일 기술자 지정
    if((sd = socket(AF_INET, SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }

    // 소켓 생성
    memset((char*)&sin,'\0',sizeof(sin)); // 메모리 초기화
    sin.sin_family = AF_INET; // 인터넷 소켓
    sin.sin_port = htons(PORTNUM); // 호스트 바이트를 네트워크 바이트로 변환
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열 형태의 ip주소를 숫자 형태로 변환

    // 소켓 bind
    if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))){
        perror("bind");
        exit(1);
    }

    // listen
    // 5개의 큐로 통신함
    if(listen(sd, 5)){
        perror("listen");
        exit(1);
    }

    // accept
    // 활성화된 클라이언트의 정보를 저장하고 ns 소켓 파일 기술자로 통신
    if((ns=accept(sd, (struct sockaddr *)&cli, &clientlen))==-1){
        perror("accept");
        exit(1);
    }
    while(1){
        // 클라이언트로 부터 받은 문자열을 출력하고
        if((n=recv(ns,buf,sizeof(buf),0)) == -1){
            perror("recv");
            exit(1);
        }

        buf[n] = '\0';
        
        if(strcmp(buf,"q")==0){
            // 클라이언트로부터 q를 받으면 소켓 종료
            send(ns,buf,strlen(msg),0);
            break;
        }

        printf("From Client : %s\n",buf);

        // 클라이언트에게 "Server received the message"문자열 전송
        if(send(ns,msg,strlen(msg),0)==-1){
            perror("send");
            exit(1);
        }
        
    }
    
    close(ns);
    close(sd);
    return 0;
}