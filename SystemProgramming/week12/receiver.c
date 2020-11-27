/* 메시지 큐 생성 및 메시지 수신하기 */
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 메시지 버퍼 정의 */
struct mymsgbuf{
    long mtype;
    char mtext[80];
};

int main(void){
    struct mymsgbuf recv_msg;
    key_t key;
    int msgid, len;

    key = ftok("keyfile",1);    //송신측과 같은 키 값 생성
    if((msgid=msgget(key,0))<0){
        perror("msgget");
        exit(1);
    }
    
    while(1){
        len = msgrcv(msgid, &recv_msg, 80, 0, 0); //메시지 수신
        printf("Received Msg = %s, Len = %d\n",recv_msg.mtext,len);
    }
    return 0;
}