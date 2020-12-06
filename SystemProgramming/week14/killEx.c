#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(void){
    printf("Before SIGCONT signal to parent.\n");
    kill(getppid(),SIGCONT); //부모프로세스에 전달 (SIGCONT : 중지된 프로새스를 재 시작)
    printf("Before SIGQUIT signal to me.\n");
    kill(getpid(),SIGQUIT); //현재 프로세스에 전달 (SIGQUIT의 기본 동작은 코어 덤프 후 종료)
    printf("After SIGQUIT signal.\n");
    
    return 0;
}