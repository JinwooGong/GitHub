#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd[2];
    pid_t pid;
    int status;

    if(pipe(fd) == -1) {  // 파이프 생성
        perror("pipe");
        exit(1);
    }

    switch(pid=fork()){ //자식 프로세스 생성
        case -1: //오류
            perror("fork");
            exit(1);
            break;
        case 0: //자식
            close(fd[1]); // fd[0]으로 읽기

            // 파이프의 입력을 표준 입력으로 변경
            dup2(fd[0], 0);
            close(fd[0]);

            // 부모프로세스로부터 파이프를 통해 받은 ps 결과를 grep 프로세스를 통해 출력
            execlp("grep", "grep", "sys", (char *)NULL);
            break;
        default: //부모
            close(fd[0]); //fd[1]로 쓰기

            // 파이프의 출력을 표준 출력으로 복사하여 변경
            dup2(fd[1], 1);
            close(fd[1]);

            //ps -ef의 결과는 파이프로 전송됨
            execlp("ps", "ps", "-ef", (char *)NULL);
            waitpid(pid, &status, 0);
            break;
    }
    return 0;
}