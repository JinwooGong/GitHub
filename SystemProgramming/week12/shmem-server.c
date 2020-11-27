#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>

#define SIZE 1024
#define FILE_SIZE 128

//세마포어 공용체를 정의한다.
union Semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
//공유 메모리
struct check_data {
    int check;
    char data[SIZE];
    char file[FILE_SIZE];
} *shared_data;

//세마포어 생성 및 초기화 함수
int initsem(key_t semkey){
    union Semun semunarg;
    int status = 0, semid;

    semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0666); //세마포어 1개 생성
    
    if(semid == -1){
        if(errno==EEXIST) //이미 존재하는 세마포어인 경우
            semid = semget(semkey, 1, 0); //이미 존재하는 식별자를 읽어옴
    }
    else{
        semunarg.val = 1; //세마포어 값을 1롤 초기화
        status = semctl(semid, 0, SETVAL, semunarg); //세마포어 semval 값을 val로 설정
    }

    if(semid == -1 || status ==-1){
        perror("initsem");
        return (-1);
    }
    return semid;
}

//세마포어 잠금 함수
int semlock(int semid){
    struct sembuf buf;

    buf.sem_num = 0;
    //음수로 설정(공유 자원을 얻고 잠금 기능을 수행하도록)
    //initsem()함수에서 초기값을 1로 했으므로 1을 빼면 0잉 되어 잠금 상태가 됨
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    if(semop(semid,&buf,1)==-1){
        perror("semlock failed");
        exit(1);
    }
    return 0;
}
//세마포어 잠금 해제 함수
int semunlock(int semid){   
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    if(semop(semid,&buf,1)==-1){
        perror("semunlock failed");
        exit(1);
    }
    return 0;
}

int main(void){
    int shmid, semid;
    void *shmaddr;
    char buf[SIZE];
    char file_name[SIZE];
    char file_buf[FILE_SIZE];
    struct shmid_ds shm_stat;
    int i,j, n, fd;
    key_t key;

    key = ftok("keyfile",1);

    if ((semid = initsem(1)) < 0) // 세마포어 생성 함수 호출
        exit(1);
    //공유메모리 생성
    if((shmid=shmget(key, sizeof(struct check_data), IPC_CREAT|0666)) == -1) {
       perror("shmid");
       exit(1);
    }

    // shmid 공유메모리를 호출 프로세스 메모리 영역으로 첨부
    shmaddr = shmat(shmid,NULL,0);

    shared_data = (struct check_data *)shmaddr;

    // check가 0이면 공유메모리에 도착한 데이터가 없다는 의미
    shared_data->check = 0;

    while(1) {
        
       //공유메모리의 check가 1이면 기다림
        while(shared_data->check==0) {
            sleep(2);
            printf("waiting...\n");
        }
        /* 공유메모리를 이용하는 프로세스가 나 혼자면 while 문 벗어남 */
        if(!strncmp(shared_data->data, "quit", 4)) {// 쓴 데이터가 ‘quit’이면 while 문 벗어남
            break;
        }
        if(!strncmp(shared_data->data,"<GET>",5)){
            memset(file_name,0,SIZE);
            for(i=0,j=6;j<strlen(shared_data->data)-1;i++,j++){
                file_name[i] = shared_data->data[j];
            }

            if(access(file_name,0)==-1){
                printf("perror");
                exit(1);
            }
            printf("File exist...\n");
            if((fd=open(file_name,O_RDONLY))==-1){
                perror("open");
                exit(1);
            }

            semlock(semid);
            while((n=read(fd,file_buf,FILE_SIZE))>0){
                printf("%s\n",file_buf);
                strncpy(shared_data->file,file_buf,SIZE);
            }
            semunlock(semid);
        }
        printf("shared memory : %s\n",shared_data->data);
        printf("input data ==> ");
        fgets(buf, SIZE, stdin);

        strncpy(shared_data->data, buf, SIZE); // 공유메모리에 데이터 쓰기
        shared_data->check = 0;

        if(!strncmp(shared_data->data, "quit", 4)) {// 쓴 데이터가 ‘quit’이면 while 문 벗어남
            break;
        }
        sleep(1);
    }

    // 공유메모리를 연결 해제
    if(shmdt(shmaddr) == -1) {
       perror("shmdt failed");
       exit(1);
    }
    // 공유메모리 삭제
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
       perror("shmctl");
       exit(1);
    }
    return 0;
}