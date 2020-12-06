#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char*argv[]){
    int fd;
    caddr_t addr;
    struct stat statbuf;

    if(argc != 2){
        fprintf(stderr,"Usage : %s filename\n",argv[0]);
        exit(1);
    }

    if(stat(argv[1],&statbuf)==-1){ //파일의 상세 정보 검색
        perror("stat");
        exit(1);
    }

    if((fd=open(argv[1],O_RDWR))==-1){
        perror("open");
        exit(1);
    }

    addr = mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)0);
    if(addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    close(fd);
    printf("%s",addr); //매핑된 내용 출력

    printf("----------\n");
    addr[0] = 'D'; //매핑된 내용 수정
    printf("%s",addr);

    msync(addr, statbuf.st_size,MS_SYNC); //수정 내용 동기화
    return 0;
}