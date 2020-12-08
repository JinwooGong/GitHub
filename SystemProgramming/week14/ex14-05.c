#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

void handler(){
    printf ("SIGALRM handler : ");
    printf("Nice to meet you\n");
}
int main(void){
    struct itimerval it;
    if(sigset(SIGALRM,handler)==SIG_ERR){
        perror("sigset");
        exit(1);
    }
    it.it_value.tv_sec = 1; //최초 시그널이 1초 후 발생
    it.it_value.tv_usec = 0;
    it.it_interval.tv_sec = 1; //타이머 간격을 1초로 설정
    it.it_interval.tv_usec = 0;

    if(setitimer(ITIMER_REAL,&it,NULL)==-1){
        perror("setitimer");
        exit(1);
    }
    while(1){
        if(getitimer(ITIMER_REAL,&it)==-1){
            perror("getitimer");
            exit(1);
        }
        pause(); //알람 시그널이 발생할때 까지 기다림
        //1초 간격으로 SIGALRM의 handler 함수 호출됨
    }
}