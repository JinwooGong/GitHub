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
int main(void){
    int pd, n;
    key_t key;
    int shmid;
    void *shmaddr;
    char s_buf[SIZE];
    char buf[SIZE];
    char file_name[SIZE];
    char temp[SIZE];
    int i,j;
    int fd;

    key = ftok("shmfile",1); //키 생성
    // 공유 메모리 설정
    shmid = shmget(key, SIZE, IPC_CREAT|0666);

    if ((pd = open("./HAN-FIFO", O_WRONLY)) == -1) {
        perror("open"); exit(1);
    }

    initscr();
    printw("Client =====\n");

    while(1){
        printw("To Server : ");
        getstr(buf);  //메세지 입력
        
        //write
        //파이프를 통해 서버에게 메세지를 보냄
        n=write(pd,buf,strlen(buf)+1);
        if(n==-1){ perror("write"); exit(1); }
        //입력한 메세지가 <EXIT>면 반복문 종료
        if(strncmp(buf,"<EXIT>",6)==0){
            printw("FIFO Closed...\n"); break;
        }
        //입력한 메세지가 <GET>이면 파일 다운로드
        if(strncmp(buf,"<GET>",5)==0){
            printw("Getting Message...\n");
            sleep(1); //서버가 공유 메모리를 쓸 때 까지 기다림
            //서버가 공유 메모리 사용을 마침
            //공유 메모리 첨부
            shmaddr = shmat(shmid,NULL,0);
            if(strncmp((char*)shmaddr,"error",5)==0){
                printw("Error : File does not exist\n");
            }
            else{
                memset(file_name,'\0',strlen(file_name)); //파일명 초기화
                strncpy(file_name,"download_",9);         //파일명 설정
                //<파일명을 temp에 저장
                for(i=0,j=6;j<strlen(buf)-1;i++,j++){
                    temp[i] = buf[j];
                }
                temp[i] = '\0';
                strcat(file_name,temp); //파일명 연결

                //다운로드 파일명 출력
                printw("File Download : %s\n",file_name);
                //파일을 읽기 전용으로 열기 (없으면 생성)
                if((fd = open(file_name,O_CREAT|O_WRONLY, 0644))==-1){
                    perror("write open");
                    exit(1);
                }
                //파일의 공유 메모리로 받은 내용 쓰기 (다운로드)
                write(fd,(char *)shmaddr,strlen(shmaddr));
                printw("Downloaded %s\n",file_name);    //다운로드 완료
                refresh();
            }
        }
        refresh();
    } endwin(); close(pd); return 0;
}
