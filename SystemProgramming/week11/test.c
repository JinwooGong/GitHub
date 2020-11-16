#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *start_thread(void * message){
    int i;
    for(i=0;i<5;i++){
        sleep(1);
        printf("%s\n",(const char*)message);
        sleep(1);
    }
    return message;
}

int main(void){
    pthread_t thing1, thing2;
    const char *message1 = "Thing 1";
    const char *message2 = "Thing 2";

    pthread_create(&thing1,NULL, start_thread, (void *)message1);
    pthread_create(&thing2,NULL, start_thread, (void *)message2);

    pthread_join(thing1,NULL);
    pthread_join(thing2,NULL);
    return 0;
}