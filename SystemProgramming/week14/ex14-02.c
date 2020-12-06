#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handler(){
    printf("SIGQUIT handler is called!\n");
}

int main(void){
    void (*hand)(int);

    hand = signal(SIGQUIT,handler);
    if(hand==SIG_ERR){
        perror("signal");
        exit(1);
    }
    printf("Wait SIGQUIT signal...\n");
    pause();
    return 0;
}