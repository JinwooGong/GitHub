#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void handler(int signo){
    char *s;

    s=strsignal(signo); //시그널 이름 리턴
    printf("Received Signal : %s\n",s); //시그널 이름 출력
}

int main(void){
    if(sigset(SIGINT,handler)==SIG_ERR){
        perror("sigset");
        exit(1);
    }

    sighold(SIGINT);    //SIGINT 시그널 블로킹 처리
    pause(); // 아무 시그널이 발생할 때 까지 대기

    return 0;
}