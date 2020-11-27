#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//세마포어 공용체를 정의한다.
union Semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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
void semhandle(){   //세마포어 생성 => 잠금 => 작업 수행(printf) => 잠금 해제
    int semid;
    pid_t pid = getpid();
    if((semid = initsem(1))< 0) //세마포어 생성 함수 호출

    semlock(semid); //세마포어 잠금 함수 호출
    printf("Lock : Process %d\n",(int)pid);
    printf("** Look Mode : Critical Section\n");
    sleep(1);
    printf("Unlock : Process %d\n",(int)pid);
    semunlock(semid);

    exit(0);
}

int main(void){
    int a;
    for(a=0;a<3;a++)
        if(fork()==0)
            semhandle();
    return 0;
}