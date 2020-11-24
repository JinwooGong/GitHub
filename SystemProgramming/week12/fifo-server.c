#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 256

int main(void) {
    int pd1, pd2, n;
    char buf[256];

    printf("Server =====\n");

    if(mkfifo("./FIFO_0", 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }
    if(mkfifo("./FIFO_1", 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }
    
    if((pd1 = open("./FIFO_1", O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    
    if((pd2 = open("./FIFO_0", O_WRONLY)) == -1) {
        perror("open");
        exit(1);
    }
    

    strcpy(buf,"Hello, FIFO");
    printf("To Client : %s\n", buf);

    n = write(pd2, buf, strlen(buf)+1);
    if (n == -1) {
        perror("write");
        exit(1);
    }
    close(pd2);

    return 0;
}