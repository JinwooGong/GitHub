#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void useResource_1(pthread_mutex_t *key){
     //LOCK
     pthread_mutex_lock(key);
     //함수를 사용중인 메세지 출력
     printf("useResource_1 function using...\n");
}
void useResource_2(pthread_mutex_t *key){
    //LOCK
    pthread_mutex_lock(key);
    //함수를 사용중인 메세지 출력
    printf("useResource_2 function using...\n");
}

//useResource_1()함수를 먼저 호출한 후 sleep후 useResource_2()함수 호출
void* th1main(void* key1){ //key1
    printf("th1main\n");
    useResource_1((pthread_mutex_t *)key1);
    sleep(1);
    useResource_2((pthread_mutex_t *)key1);
    //UNLOCK key1
    pthread_mutex_unlock(key1);

    return key1;
}
//useResource_2()함수를 먼저 호출한 후 sleep후 useResource_1()함수 호출
void* th2main(void* key2){ //key2
    printf("th2main\n");
    useResource_2((pthread_mutex_t *)key2);
    sleep(1);
    useResource_1((pthread_mutex_t *)key2);
    //UNLOCK key2
    pthread_mutex_unlock(key2);

    return key2;
}

int main(void){
    pthread_t th1, th2;
    pthread_mutex_t key1, key2;

    //key1, key2뮤텍스 생성
    pthread_mutex_init((pthread_mutex_t *)&key1,NULL);
    pthread_mutex_init((pthread_mutex_t *)&key2,NULL);

    //스레드 생성
    pthread_create(&th1,NULL,th1main,(void *)&key1);
    pthread_create(&th2,NULL,th2main,(void *)&key2);

    //스레드가 종료될 때 까지 기다림
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    return 0;
}