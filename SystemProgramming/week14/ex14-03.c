#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    sigset_t st;
    int i;
    sigemptyset(&st);
    sigaddset(&st, SIGINT); //SIGINT를 시그널 집합에 추가
    
    sigprocmask(SIG_BLOCK,&st,NULL); //시그널 집합에 대해 블로킹 설정
    
    for(i=0;i<10;i++){
        printf("Hallym SW\n");
        sleep(1);
    }
    sigprocmask(SIG_UNBLOCK,&st,NULL); //시그널 집합에 대해 블로킹 해제
}