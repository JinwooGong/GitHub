#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void useResource_1(pthread_mutex_t *key){
     //LOCK
     pthread_mutex_lock(key);
     printf("useResource_1 function using...\n");
}
void useResource_2(pthread_mutex_t *key){
    //LOCK
    pthread_mutex_lock(key);
    printf("useResource_2 function using...\n");
}

void* th1main(void* key1){ //key1
    useResource_1((pthread_mutex_t *)key1);
    sleep(1);
    useResource_2((pthread_mutex_t *)key1);
    //UNLOCK key1
    pthread_mutex_unlock(key1);

    return key1;
}
void* th2main(void* key2){ //key2
    useResource_2((pthread_mutex_t *)key2);
    sleep(1);
    useResource_1((pthread_mutex_t *)key2);
    //UNLOCK; key2
    pthread_mutex_unlock(key2);

    return key2;
}

int main(void){
    pthread_t th1, th2;
    pthread_mutex_t *key1, *key2;
    pthread_mutex_init(key1,NULL);
    pthread_mutex_init(key2,NULL);

    pthread_create(&th1,NULL,th1main,(void *)key1);
    pthread_create(&th2,NULL,th2main,(void *)key2);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    return 0;
}