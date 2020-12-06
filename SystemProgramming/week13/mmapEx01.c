#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    int fd;
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
    printf("%s",addr); //매핑한 파일에 저장된 내용을 읽어서 터미널에 출력(read)
    return 0;
}