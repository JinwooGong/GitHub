#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    sigset_t st;

    sigemptyset(&st); //시그널 집합 비우기
    
    /* 시그널 추가 */
    sigaddset(&st, SIGINT);
    sigaddset(&st,SIGQUIT);

    if(sigismember(&st,SIGINT)) //SIGINT가 설정되어 있는지 여부를 확인
        printf("SIGINT has been set.\n");
    
    return 0;
}