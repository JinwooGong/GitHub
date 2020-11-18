#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

struct account{
    int balance;    //통장 잔고
    int saving;     //인출 가능한 금액
};

void disburse_money(struct account * account,int amount, char *tname){
    account->saving -= amount;      //돈을 인출
    if(account->saving <= 0){       //인출 후 잔액이 마이너스일 경우
        account->saving += amount;  //인출 잔액을 다시 되돌린 후 에러 메세지 출력
        printf("%s : 인출할 수 없습니다. ",tname);
        printf("통장 잔액 : %d\n",account->saving);
        return;
    }
    printf("%s : %d 인출됨\n",tname,amount);
}

int withdraw(struct account * account, int amount, char* tname){
    //(1) 잔액 확인
    int balance = account->balance;

    //인출이 가능한지 확인
    if(balance < amount) return -1;
    sleep(1);

    printf("%s : 인출 가능\n",tname);
    //(2) 통장 잔고를 업데이트
    account -> balance = balance - amount;
    printf("%s : 통장 잔고 : %d\n",tname, account->balance);
    //(3) 돈을 인출함
    disburse_money(account,amount, tname);
}

//서로 다른 이름과 인출금액을 전달 하기 위해 스레드 함수를 2개 생성
void * start_withdraw1(void * account){
    char *tname = "th1";
    withdraw((struct account *)account,300,tname);
    return account;
}
void * start_withdraw2(void * account){
    char *tname = "th2";
    withdraw((struct account *)account,400,tname);
    return account;
}

int main(void){
    struct account acc;
    pthread_t th1, th2;
    //통장 잔액과 인출 가능한 금액 초기화
    acc.saving=500;
    acc.balance=500;

    pthread_create(&th1,NULL,start_withdraw1,(void *)&acc);
    pthread_create(&th2,NULL,start_withdraw2,(void *)&acc);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    return 0;
}



