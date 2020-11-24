#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 256
int main(void){
    int pd1,pd2, n;
    char buf[SIZE]; 
    if ((pd1 = open("./FIFO_0", O_RDONLY)) == -1) {
        perror("open"); exit(1);
    }
    
    if((pd2 = open("./FIFO_1", O_WRONLY)) == -1) {
        perror("open");
        exit(1);
    }
    
    printf("Client =====\n");
    write(1, "From Server : ", 13);
    while((n=read(pd1, buf, SIZE)) > 0)
        write(1, buf, n);
    if(n == -1) {
        perror("read");
        exit(1);
    }
    write(1, "\n", 1);
    close(pd1);
    return 0;
}
