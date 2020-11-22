#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void){
    FILE *fp;
    char buf[256];

    //읽기모드(r)로 파이프 생성
    //자식프로세스는 date 명령어 실행
    fp = popen("date","r");
    if(fp == NULL){
        perror("popen failed");
        exit(1);
    }

    if(fgets(buf,sizeof(buf),fp) == NULL){
        perror("No data from pipe");
        exit(1);
    }
    printf("Recv : %s\n",buf);
    return 0;
}