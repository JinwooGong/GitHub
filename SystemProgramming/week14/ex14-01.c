#include <signal.h>
#include <stdio.h>

int main(void){
    sigset_t st;

    sigfillset(&st); // 모든 시그널을 집합에 설정

    if(sigismember(&st,SIGBUS)) //SIGBUS가 설정되어 있는지 여부를 확인
        printf("SIGBUS is set.\n");
    else
        printf("SIGBUS is not set.\n");
    return 0;
}