#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <dirent.h>

#define SOCKET_NAME "practice_socket"
#define SIZE 1024

int main(void){
    char buf[SIZE];
    char dir_path[SIZE];
    char file_name[SIZE];
    

    DIR *dp;
    struct dirent *dent;

    struct sockaddr_un ser, cli;
    int sd, nsd, len, clen;

    int i, d_index, f_index=0;
    int n, fd, f_flag=0;

    if((sd = socket(AF_UNIX,SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    memset((char *)&ser, 0 , sizeof(struct sockaddr_un));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path,SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    if(bind(sd, (struct sockaddr*)&ser, len)){
        perror("bind");
        exit(1);
    }
    if(listen(sd, 5) < 0){
        perror("listen");
        exit(1);
    }
    
        
    if((nsd = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
        perror("accept");
        exit(1);
    }
    while(1){
        
        printf("Waiting...\n");
        if(recv(nsd, dir_path, sizeof(dir_path),0)==-1){
            perror("recv");
            exit(1);
        }   
        if(strcmp(dir_path,"q")==0){
            break;
        }
        
        for(i=strlen(dir_path)-1;i>=0;i--){
            if(dir_path[i] == '/'){
                d_index = i;
                break;
            }
        }
        f_index = 0;
        while(i < strlen(dir_path)){
            file_name[f_index++] = dir_path[i+1];
            i++;
        }
        dir_path[d_index] = '\0';
        file_name[f_index] = '\0';        

        printf("directory path : %s\n",dir_path);
        printf("file name : %s\n",file_name);

        chdir(dir_path);
        
        if((dp = opendir("."))==NULL){
            perror("opendir");
            exit(1);
        }

        while((dent=readdir(dp))){
            if(strcmp(dent->d_name,file_name)==0){
                f_flag=1;
                if((fd=open(file_name,O_RDONLY))==-1){
                    perror("oepn");
                    exit(1);
                }
                while((n=read(fd,buf,SIZE))>0){
                    if(send(nsd,buf,n,0)==-1){
                        perror("send");
                        exit(1);
                    }
                }
                close(fd);
                break;
            }
        }
        if(!f_flag){
            if(send(nsd,"File not exist",14,0)==-1){
                perror("send");
                exit(1);
            }
        }
        closedir(dp);
    }
    close(nsd);
    close(sd);
    return 0;
}