#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

struct StudentInfo{
    char name[BUFSIZ];
    char major[BUFSIZ];
    char univ[BUFSIZ];
};


void * start_thread(void *std){
    int i;
    struct StudentInfo *sinfo = (struct StudentInfo *)std;
    for(i=0;i<10;i++){
        //sleep(1);
        printf("Name:%s, Major:%s, Univ:%s\n",sinfo->name,sinfo->major,sinfo->univ);
        //sleep(1);
    }

    return std;
}

int main(void){
    pthread_t th1, th2;
    struct StudentInfo std1, std2;

    strcpy(std1.name,"Brad Pitt");
    strcpy(std1.major,"Computer Engineering");
    strcpy(std1.univ,"Holly University");

    strcpy(std2.name,"Angelina Jollie");
    strcpy(std2.major,"Computer Science");
    strcpy(std2.univ,"Wood University");

    pthread_create(&th1,NULL,start_thread,(void *) &std1);
    pthread_create(&th2,NULL,start_thread,(void *) &std2);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}