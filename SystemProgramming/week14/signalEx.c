#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handler(int signo){
    printf("Signal Handler Signal Number : %d\n",signo);
    psignal(signo,"Received Signal");
}

int main(void){
    void (*hand)(int);
    hand = signal(SIGINT, handler); // 시그널 핸들러 지정
    if(hand == SIG_ERR){
        perror("signal");
        exit(1);
    }

    printf("Wait 1st Ctrl+C... : SIGINT\n");
    pause(); //시그널이 입력 되기를 기다림
    printf("After 1st Signal Handler\n");
    printf("Wait 2nd Ctrl+C... : SIGINT\n");
    pause();
    printf("After 2nd Signal Handler\n");
    return 0;
}