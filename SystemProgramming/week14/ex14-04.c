#include <unistd.h>
#include <signal.h>
#include <stdio.h>

sigset_t st;
void handler(){
        /* 메시지 출력 */ 
        printf("At handler : Wake up!\n");

        /* SIGINT 를 제외한 모든 시그널 블로킹*/
        sigfillset(&st);        //모든 시그널을 집합에 추가
        sigdelset(&st,SIGINT);  //집합에서 SIGINT 제외
        sigprocmask(SIG_BLOCK,&st,NULL); //집합의 모든 시그널 블로킹
}

int main(void) {
        signal(SIGALRM, handler);
        for( int i = 0 ; i < 5 ; i++ ) {
                alarm(2); // 2초 설정
                printf("Pause ...\n");
                pause();
        }
        printf("Bye~\n");
        return 0;
}
