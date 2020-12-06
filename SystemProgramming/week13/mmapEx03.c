#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    int fd, pagesize, length;
    caddr_t addr;

    pagesize = sysconf(_SC_PAGESIZE); //메모리의 [페이지 크기] 정보 검색
    length = 1 * pagesize;

    if((fd=open("m.dat",O_RDWR|O_CREAT|O_TRUNC,0666))==-1){
        perror("open");
        exit(1);
    }

    if(ftruncate(fd,(off_t)length) == -1){ //빈 파일의 크기 증가
        perror("ftruncate");
        exit(1);
    }
    //파일의 크기가 > 0 이므로 메모리 매핑 mmap함수 호출 가능
    addr = mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(off_t)0);

    if(addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    close(fd);
    strcpy(addr, "ftruncate Test\n"); //매핑한 메모리에 데이터 쓰기 => 파일에 데이터 쓰기
    
    return 0;
}