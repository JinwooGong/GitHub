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
    char buf[SIZE];         //파일 내용
    char dir_path[SIZE];    //디렉토리 경로
    char file_name[SIZE];   //파일 명
    
    DIR *dp;
    struct dirent *dent;

    struct sockaddr_un ser, cli;
    int sd, nsd, len, clen;

    int i, d_index, f_index=0;
    int n, fd, f_flag=0; //f_flag : 파일 발견 flag (찾으면 1)

    //socket
    if((sd = socket(AF_UNIX,SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    
    //구조체 설정
    memset((char *)&ser, 0 , sizeof(struct sockaddr_un));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path,SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    //bind
    if(bind(sd, (struct sockaddr*)&ser, len)){
        perror("bind");
        exit(1);
    }
    //listen
    if(listen(sd, 5) < 0){
        perror("listen");
        exit(1);
    }
    
    //accet
    if((nsd = accept(sd, (struct sockaddr *)&cli, &clen))==-1){
        perror("accept");
        exit(1);
    }
    while(1){
        
        printf("Waiting...\n");
        //recv
        if(recv(nsd, dir_path, sizeof(dir_path),0)==-1){
            perror("recv");
            exit(1);
        }
        // q 입력 시 반복문 종료
        if(strcmp(dir_path,"q")==0){
            break;
        }
        
        // 파일 명 앞의 '/'의 index값을 가져옴
        for(i=strlen(dir_path)-1;i>=0;i--){
            if(dir_path[i] == '/'){
                d_index = i;
                break;
            }
        }
        f_index = 0;
        // 파일명만 따로 추출
        while(i < strlen(dir_path)){
            file_name[f_index++] = dir_path[i+1];
            i++;
        }
        
        dir_path[d_index] = '\0'; // 파일명을 제외한 디렉토리 path
        file_name[f_index] = '\0';        

        printf("directory path : %s\n",dir_path);
        printf("file name : %s\n",file_name);

        chdir(dir_path);
        
        // 디렉토리 open
        if((dp = opendir("."))==NULL){
            perror("opendir");
            exit(1);
        }

        // 디렉토리 내의 파일 정보 가져오기
        while((dent=readdir(dp))){
            if(strcmp(dent->d_name,file_name)==0){ //파일명 비교
                f_flag=1; //파일 발견 flag
                if((fd=open(file_name,O_RDONLY))==-1){ //파일 open
                    perror("oepn");
                    exit(1);
                }
                while((n=read(fd,buf,SIZE))>0){ //파일 내용 read
                    if(send(nsd,buf,n,0)==-1){ //파일 내용 send
                        perror("send");
                        exit(1);
                    }
                }
                close(fd); //파일 기술자 close
                break;
            }
        }
        if(!f_flag){ //파일 발견 flag가 0이면 실행
            if(send(nsd,"File not exist",14,0)==-1){
                perror("send");
                exit(1);
            }
        }
        closedir(dp); //디렉토리 close
    }
    close(nsd);
    close(sd);
    return 0;
}