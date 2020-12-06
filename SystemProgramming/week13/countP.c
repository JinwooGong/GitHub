#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    int fd;
    int pcnt=0,ccnt=0;
    caddr_t addr;
    struct stat statbuf;

    if(argc!=2){
        fprintf(stderr,"Usage : %s filename\n",argv[0]);
        exit(1);
    }

    if(stat(argv[1], &statbuf)==-1){ //파일 상태 조회
        perror("stat");
        exit(1);
    }

    if((fd = open(argv[1], O_RDWR))==-1){
        perror("open");
        exit(1);
    }
    //파일 내용을 메모리에 매핑
    addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);
    if(addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    close(fd);
    for(int i = 0;i<100;i++){
        if(addr[i]=='p') pcnt++;
        else ccnt++;
    }
    printf("게임 결과 : %s\n",addr);
    printf("p : %d, c : %d\n",pcnt, ccnt);
    if(pcnt>50)
        printf("Parent won!\n");
    else if(pcnt < 50)
        printf("Child won!\n");
    else
        printf("Even!!\n");
    
    return 0;
}