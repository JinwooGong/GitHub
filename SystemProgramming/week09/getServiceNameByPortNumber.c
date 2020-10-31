#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int main(int argc, char *argv[]){
    struct servent *sent; //servent 구조체 생성

    if(argc != 2){
        printf("Input 2 arguments\n");
        exit(1);
    }

    setservent(0); // 데이터베이스 read

    sent = getservbyport(atoi(argv[1]),"tcp"); // port번호에 일치하는 구조체를 sent에 저장
    if(sent==NULL){ // 일치하는 포트번호가 없음
        perror("getservbyport");
        exit(1);    
    }
    printf("Service Name=%s, Port=%d\n",sent->s_name,sent->s_port);
    endservent(); // 데이터베이스 close
    return 0;
}