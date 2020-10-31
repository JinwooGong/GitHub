#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[]){
    struct servent *sent; // servent 구조체 생성

    if(argc!=2){
        printf("Input 2 arguments\n");
        exit(1);
    }

    setservent(0); // 데이터베이스 읽기 시작

    sent = getservbyname(argv[1],"tcp");

    if(sent==NULL){ // 데이터베이스에 찾는값이 없을경우
        perror("getservbyname");
        exit(1);
    }
    printf("Service Name=%s, Port=%d\n", sent->s_name,sent->s_port);
    endservent(); // 데이터베이스 close
    return 0;
}