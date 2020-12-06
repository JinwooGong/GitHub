#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    int fd;
    pid_t pid;
    caddr_t addr;
    struct stat statbuf;

    if(argc != 2){
        fprintf(stderr,"Usage : %s filename\n",argv[0]);
        exit(1);
    }
    if(stat(argv[1],&statbuf)==-1){
        perror("stat");
        exit(1);
    }
    if((fd = open(argv[1],O_RDWR))==-1){
        perror("open");
        exit(1);
    }
    //메모리 매핑
    addr = mmap(NULL, statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)0);
    if(addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    close(fd);

    switch(pid=fork()){
        case -1: /* fork failed */
            perror("fork");
            exit(1);
            break;
        case 0: /* child process */
            printf("1. Child Process : addr = %s",addr);
            sleep(1);
            addr[0] = 'x'; //자식 프로세스가 매핑된 내용 수정
            printf("2. Child Process : addr = %s",addr);
            sleep(2);
            printf("3. Child Process : addr = %s",addr);
            break;
        default:    /* parent process */
            printf("1. Parent Process : addr = %s",addr);
            sleep(2);
            printf("2. Parent Process : addr = %s",addr);
            addr[1] = 'y'; //부모 프로세스가 매핕된 내용 수정
            printf("3. Parent Process : addr = %s",addr);
            waitpid(pid,NULL,0);
            break;
    }
    return 0;
}