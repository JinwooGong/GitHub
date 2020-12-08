#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handler(){
    printf("SIGQUIT handler is called!\n");
}

int main(void){
    void (*hand)(int);

    hand = signal(SIGQUIT,handler); //SIGQUIT 시그널에 핸들러 등록
    if(hand==SIG_ERR){
        perror("signal");
        exit(1);
    }
    printf("Wait SIGQUIT signal...\n");
    pause(); //시그널 입력을 기다림
    return 0;
}