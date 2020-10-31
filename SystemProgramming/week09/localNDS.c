#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    struct hostent *hent;
    char search_host[20];
    int i=0;

    strcpy(search_host,argv[1]);
    sethostent(0);

    //hent = gethostbyname(argv[1]); // dns까지 검색

    while((hent=gethostent())!=NULL){
        if(strcmp(search_host,hent->h_name)==0){
            while(hent->h_addr_list[i]){ // ip 주소 배열에서 값을 가져옴
                // 구조체 형태의 ip 주소를 문자열로 변환
                printf("IP addr %d %s\n", i+1, inet_ntoa(*(struct in_addr*)hent->h_addr_list[i]));
                i++;
            }
            break;
        }
    }
    if(hent==NULL){
        printf("No Search %s\n",argv[1]);
        exit(1);
    }

    endhostent();
    return 0;
}