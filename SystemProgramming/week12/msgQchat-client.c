/* 메시지 큐 생성 및 메시지 수신하기 */
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define SIZE 1024

/* 메시지 버퍼 정의 */
struct msgbuf{
    long mtype;
    char mtext[80];
};

struct msgbuf s_msg;
struct msgbuf r_msg;
char buf[SIZE];

int main(void){
    key_t s_key, r_key;
    pthread_t s_th, r_th;
    int sm_id,rm_id;
    pid_t pid;
    
    //송신측과 같은 키 값 생성
    r_key = ftok("keyfile1",1);
    s_key = ftok("keyfile2",2);   

    
    //메시지를 받기 위한 메시지 큐 식별자 생성
    if((rm_id= msgget((key_t)r_key,0))<0){
        perror("msgget");
        exit(1);
    }
    if((sm_id= msgget((key_t)s_key,0))<0){
        perror("msgget");
        exit(1);
    }
    

    switch(pid=fork()){
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            //메시지를 받기 위한 메시지 큐 생성
            //rm_id = msgget((key_t)r_key, IPC_CREAT|0644);
            if(rm_id==-1){
                perror("msgget");
                exit(1);
            }
            while(1){
                msgrcv((int)rm_id, &r_msg, SIZE, 0, 0); //메시지 수신
                printf("Received Msg : %s\n",r_msg.mtext);
            }
        default:
            s_msg.mtype = 2; //s_msg구조체의 유형을 2로 정의

            while(1){
                printf("Input >>\n");
                fgets(buf,SIZE,stdin);
                strcpy(s_msg.mtext, buf); //보낼 메시지
                
                if(msgsnd(sm_id,(void *)&s_msg, SIZE ,IPC_NOWAIT)==-1){
                    perror("msgsnd");
                    exit(1);
                }
            }
            break; 
    }
    
    
    return 0;
}