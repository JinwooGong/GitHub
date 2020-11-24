#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    int fd[2]; //파일 기술자 2개
    pid_t pid;
    char buf[257];
    int len, status;

    if(pipe(fd)==-1){
        perror("pipe");
        exit(1);
    } 
    switch(pid = fork()){
        case -1: //오류
            perror("fork");
            exit(1);
            break;
        case 0: //자식
            // 파이프에서 읽을 것이므로 쓰기용 파일기술자(fd[1])를 닫는다.
            close(fd[1]);
            write(1,"Child Process:",15);
            len = read(fd[0],buf,256); //파이프에서 읽기
            write(1, buf,len);
            close(fd[0]);
            break;
        default: //부모
            // 파이프에서 쓸 것이므로 읽기용 파일기술자(fd[0])를 닫는다.
            buf[0] = '\0';
            write(fd[1],"Test Message\n",14); //파이프에 텍스트 쓰기
            close(fd[1]);
            waitpid(pid, &status, 0);
            close(fd[0]);

    }
    return 0;
}