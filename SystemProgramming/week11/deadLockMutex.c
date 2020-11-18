#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

pthread_mutex_t gkey; //global mutex

void useResource_1(pthread_mutex_t *key){
     pthread_mutex_lock(key);   //LOCK
     printf("useResource_1 function using...\n");
}
void useResource_2(pthread_mutex_t *key){
    pthread_mutex_lock(key);    //LOCK
    printf("useResource_2 function using...\n");
}

void* th1main(void* key1){ //key1

    printf("th1main\n");
    pthread_mutex_lock(&gkey); //GLOBAL LOCK
    useResource_1((pthread_mutex_t *)key1);
    pthread_mutex_unlock(key1);
    sleep(1);

    useResource_2((pthread_mutex_t *)key1);

    pthread_mutex_unlock(key1);     //UNLOCK key1
    pthread_mutex_unlock(&gkey);    //GLOBAL UNLOCK
    return key1;
}
void* th2main(void* key2){ //key2

    printf("th2main\n");
    pthread_mutex_lock(&gkey);      //GLOBAL LOCK
    useResource_2((pthread_mutex_t *)key2);
    pthread_mutex_unlock(key2);
    sleep(1);

    useResource_1((pthread_mutex_t *)key2);

    pthread_mutex_unlock(key2);     //UNLOCK key2
    pthread_mutex_unlock(&gkey);    //GLOBAL UNLOCK

    return key2;
}

int main(void){
    pthread_t th1, th2;
    pthread_mutex_t key1, key2;

    //global mutex 생성
    pthread_mutex_init((pthread_mutex_t *)&gkey,NULL);

    //key1, key2 mutex 생성
    pthread_mutex_init((pthread_mutex_t *)&key1,NULL);
    pthread_mutex_init((pthread_mutex_t *)&key2,NULL);

    pthread_create(&th1,NULL,th1main,(void *)&key1);
    pthread_create(&th2,NULL,th2main,(void *)&key2);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    return 0;
}