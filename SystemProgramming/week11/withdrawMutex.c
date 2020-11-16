#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

pthread_mutex_t mutex_var;

struct account{
    int balance;
    int saving;
    pthread_mutex_t mutex;
};

void disburse_money(struct account * account,int amount, char *tname){
    account->saving -= amount;
    if(account->saving <= 0){
        account->saving += amount;
        printf("%s : 인출할 수 없습니다. ",tname);
        printf("통장 잔액 : %d\n",account->saving);
        return;
    }
    printf("%s : %d 인출됨\n",tname,amount);
}

int withdraw(struct account * account, int amount, char* tname){
    pthread_mutex_lock(& account->mutex); //잠금
    //(1) 잔액 확인
    int balance = account->balance;

    //인출이 가능한지 확인
    if(balance < amount) {
        printf("%s : 인출 불가능\n",tname);
        pthread_mutex_unlock(&account->mutex); //잠금 해제
        return -1;
    }
    sleep(1);
    printf("%s : 인출 가능\n",tname);

    //(2) 통장 잔고를 업데이트
    account -> balance = balance - amount;
    printf("%s : 통장 잔고 : %d\n",tname, account->balance);
    //(3) 돈을 인출함
    disburse_money(account,amount, tname);

    pthread_mutex_unlock(&account->mutex); //잠금 해제
    return 0;
}

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
    acc.saving=500;
    acc.balance=500;
    
    pthread_mutex_init((pthread_mutex_t *)&acc.mutex,NULL);

    char *tname1 = "th1";
    char *tname2 = "th2";

    pthread_create(&th1,NULL,start_withdraw1,(void *)&acc);
    pthread_create(&th2,NULL,start_withdraw2,(void *)&acc);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    return 0;
}


