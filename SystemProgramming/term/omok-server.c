#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNUM 9010

//the thread function
void *client_manager(void *);

static int turn = 0;
static int next_turn = 0; //다음 순서

static int first_sock;
static int second_sock;

int main(int argc , char *argv[])
{
    int sd , clen;
    int client_sock[2]; //2명의 클라이언트 소켓을 받을 변수
    struct sockaddr_in server , client;

    pthread_t thread_id;

    //Create socket
    if((sd = socket(AF_INET , SOCK_STREAM , 0))==-1){
        perror("socket"); exit(1);
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(PORTNUM);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Bind
    if( bind(sd,(struct sockaddr *)&server , sizeof(server)) == -1){
        perror("bind failed. Error"); exit(1);
    }
    puts("bind done");

    //Listen
    if(listen(sd , 3)<0){
        perror("listen"); exit(1);
    }

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    clen = sizeof(struct sockaddr_in);

    while( (client_sock[turn] = accept(sd, (struct sockaddr *)&client, &clen)) )
    {
        if (client_sock[turn] < 0){
            perror("accept"); exit(1);
        }
        puts("Connection accepted");

        //클라이언트를 제어하기 위한 스레드 생성
        //인자로 클라이언트와 연결된 소켓을 전달
        if( pthread_create(&thread_id , NULL , client_manager , (void*) &client_sock[turn]) == -1){
            perror("pthread_create"); exit(1);
        }

        if(turn ==0) {
            first_sock = client_sock[turn];
            turn =1;
        }
        else if(turn==1){
            second_sock = client_sock[turn];
            turn =0;

        }
    }

    return 0;
}


void *client_manager(void *socket)
{
    int sock = *(int*)socket; //인자로 전달받은 소켓
    int n; //읽은 메세지의 크기를 저장할 변수
    char client_message[BUFSIZ];

    char temp[BUFSIZ];
    int x, y, c;
    int xyc[3];
    int channel;

    memset(client_message, 0, BUFSIZ);

    if(turn==1) { //0번째 턴인 경우 turn = 1인 상태
        //turn 정보를 클라이언트에게 전송
        channel = 0;
        client_message[0]='0';
        client_message[1]='\n';
        write(sock , client_message , strlen(client_message));
        turn=1;
    }
    else {  //1번째 턴인 경우 turn=0인 상태
        //turn 정보를 클라이언트에게 전송
        channel = 1;
        client_message[0]='1';
        client_message[1]='\n';
        write(sock , client_message , strlen(client_message));
        turn=0;
    }

    //클라이언트로부터 좌표값과 차례를 받음
    while((n = recv(sock , client_message , BUFSIZ , 0)) > 0 )
    {
        client_message[n] = '\0';

        memcpy(temp, client_message, sizeof(client_message));  //받은 메세지를 temp로 복사

        x = atoi(temp)/100;
        y = atoi(&temp[1])/10%100;
        c = atoi(&temp[2])%10;
        printf("x: %d, y: %d, c: %d\n", x, y, c);

        if(next_turn==channel) { //턴이 맞으면 수행
            //전달받은 클라이언트에게 다시 보냄
            write(sock , client_message , strlen(client_message));

            if(channel==0){ //상대방에게 정보를 보냄
                write(second_sock , client_message , strlen(client_message));
            }
            else {
                write(first_sock , client_message , strlen(client_message));
            }

            //다음 턴
            next_turn++;
            next_turn = next_turn %2;
        }

        memset(client_message, 0, BUFSIZ);

    }
    if(n == 0){
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(n == -1){
        perror("recv");
    }
    return socket;
}
