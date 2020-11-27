#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <curses.h>

#define SIZE 1024

int main(void) {
    int pd, n;
    int i,j, fd;
    key_t key;
    int shmid;
    void *shmaddr;
    char buf[SIZE];
    char s_buf[SIZE];
    char file_name[SIZE];
    char file_buf[SIZE];
    sigset_t mask;

    
    key = ftok("shmfile", 1); // 키 생성
    // 공유 메모리 생성
    shmid = shmget(key, SIZE, IPC_CREAT|0666); 
    initscr();
    clear();
    printw("Listener wait for Client\n");
    refresh();
    //이름있는 파이프 생성
    /*
    if(mkfifo("HAN-FIFO", 0644) == -1) {
        perror("mkfifo");
        exit(1);
    }*/
    //파이프 열고 다른 프로세스의 연결을 기다림
    if((pd = open("./HAN-FIFO", O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    
    //다른 프로세스가 연결되면 문장 출력
    clear();
    printw("Listener Start =====\n");
    refresh();
    while(1){
        printw("From Client : ");
        //read
        n=read(pd,buf,SIZE);
        if(n == -1) { perror("read"); exit(1); }
        printw("%s\n",buf);
        refresh();
        if(strncmp(buf,"<EXIT>",6)==0){
            printw("FIFO Closed...\n");
            refresh();
            sleep(5);
            break;
        }
        if(strncmp(buf,"<GET>",5)==0){
            printw("11\n");
            memset(file_name,0,SIZE);
            for(i=0,j=6;j<strlen(buf)-1;i++,j++){
                file_name[i] = buf[j];
            }
            printw("11\n");
            printw("File : %s\n",file_name);
            if((access(file_name,0))==0){
                if((fd=open(file_name,O_RDONLY))==-1){
                    perror("open");
                    exit(1);
                }
                if((n=read(fd,file_buf,SIZE))>0){
                    //공유 메모리 첨부
                    shmaddr = shmat(shmid, NULL, 0);
                    // 공유 메모리에 데이터 기록하기
                    strcpy(shmaddr, file_buf); 
                }
                printw("\nFile Message : %s\n",(char *)shmaddr);
            }
            else{
                strncpy((char*)shmaddr, "error",5);
            }
        }
    }
    endwin();
    close(pd);
    return 0;
}