#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    int crnd, prnd;
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
    if((fd=open(argv[1],O_RDWR))==-1){
        perror("open");
        exit(1);
    }
    
    //메모리 매핑
    addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);
    if(addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    close(fd);
    printf("%s\n",addr);

    switch(pid=fork()){
        case -1:    /* fork failed */
            perror("fork");
            exit(1);
            break;
        case 0:     /* child process */
            for (int i = 0; i < 100; i++) { // 10번 반복
                addr[i] = 'c';
                sleep(1);
	        }
            printf("\n");
            break;
        default:    /* parent process */
            srand(time(NULL)); // 매번 다른 시드값 생성
            for (int i = 0; i < 100; i++) { // 10번 반복
                prnd = rand()%100; // 난수 생성
                printf("(p)%d, ",prnd);
                if(i%9==0&&i!=0) printf("\n");
                addr[prnd] = 'p';
                sleep(1);
	        }
            printf("\n");
            waitpid(pid,NULL,0);
            break;
    }
    return 0;
}