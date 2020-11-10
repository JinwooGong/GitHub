#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void){
    char buf[BUFSIZ];

    struct tm *tm;
    time_t t;

    time(&t);
    tm = localtime(&t);
    strftime(buf, sizeof(buf), "%G년, %m월 %d일 %H:%M:%S", tm);
    printf("%s\n",buf);
    sleep(1);
    return 0;
}