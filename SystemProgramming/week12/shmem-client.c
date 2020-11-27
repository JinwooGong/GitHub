#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>

#define SIZE 1024
#define FILE_SIZE 128

struct check_data {
    int check;
    char data[SIZE];
    char file[FILE_SIZE];
} *shared_data;

int main(void){
    int i,j;
    int shmid, fd;
    void *shmaddr;
    char buf[SIZE];
    char file_name[SIZE];
    char file_buf[FILE_SIZE];
    char temp[SIZE];
    struct shmid_ds shm_stat;

    key_t key;

    key = ftok("keyfile",1);

    if((shmid=shmget(key, SIZE, IPC_CREAT|0666)) == -1) {
       perror("shmid failed");
       exit(1);
    }

    // 공유 메모리 연결
    shmaddr = shmat(shmid,NULL,0);

    shared_data = (struct check_data *)shmaddr;
    
    while(1) {
        //공유메모리의 check가 1이면 기다림
        while(shared_data->check) {
            sleep(2);
            printf("waiting...\n");
        }
        if(!strncmp(shared_data->data, "quit", 4)) {// 쓴 데이터가 ‘quit’이면 while 문 벗어남
            break;
        }
        printf("shared memory : %s\n",shared_data->data);
        printf("input data ==> ");
        fgets(buf, SIZE, stdin);

        strncpy(shared_data->data, buf, SIZE); // 공유메모리에 데이터 쓰기
        shared_data->check = 1;
        if(!strncmp(shared_data->data, "quit", 4)) {// 쓴 데이터가 ‘quit’이면 while 문 벗어남
            break;
        }
        if(!strncmp(shared_data->data,"<GET>",5)){
            memset(file_name,'\0',strlen(file_name));
            memset(temp,'\0',strlen(temp));
            strncpy(file_name,"download_",9);
            
            for(i=0,j=6;j<strlen(shared_data->data)-1;i++,j++){
                temp[i] = shared_data->data[j];
            }
            strcat(file_name,temp);
            printf("%s Downloading...\n",file_name);
            sleep(1); //파일이 올라올 때 까지 기다림
            if((fd=open(file_name,O_CREAT|O_WRONLY,0644))==-1){
                perror("open");
                exit(1);
            }
            write(fd,shared_data->file,strlen(shared_data->file));
            printf("Download Complete...\n");
        }
        sleep(1);
    }

    // 공유메모리를 호출 프로세스의 메모리 영역에서 분리
    if(shmdt(shmaddr) == -1) {
       perror("shmdt");
       exit(1);
    }
    // 공유 메모리 삭제
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
       perror("shmctl");
       exit(1);
    }
    exit(0);

}