#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void handler(){
    sigset_t st;
    
    sigfillset(&st);
    sigdelset(&st,SIGINT);
    sigdelset(&st,SIGALRM);
    sigprocmask(SIG_BLOCK,&st,NULL);
    
    printf("At handler : Wake up!\n");
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
