#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){

    int fd1[2], fd2[2];
    pid_t pid;
    char buf[256];
    int len, status;

    /* 파이프 세트 2개 생성 */
    if(pipe(fd1)==-1){
        perror("pipe");
        exit(1);
    }
    if(pipe(fd2)==-1){
        perror("pipe");
        exit(1);
    }

    switch(pid=fork()){
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            close(fd1[1]); //fd1[0]으로 읽기
            close(fd2[0]); //fd2[1]로 쓰기
            write(1,"Child Porcess: ",15);  //터미널에 출력
            len = read(fd1[0],buf, 256);    //파이프에서 읽기
            write(1,buf,len);               //읽은 내용을 터미널에 출력

            strcpy(buf,"Good\n");
            write(fd2[1],buf, strlen(buf)); //파이프에 쓰기
            break;
        default:
            close(fd1[0]); //fd1[1]으로 쓰기
            close(fd2[1]); //fd2[0]로 읽기

            buf[0] = '\0';
            write(fd1[1],"Hello\n",6);
            sleep(1);
            write(1,"Parent Process: ",15);
            len =read(fd2[0], buf, 256);
            write(1,buf,len);
            waitpid(pid,&status, 0);
            break;
    }

    return 0;
}