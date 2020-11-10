#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#define PORTNUM 9005

int main(void)
{
  int iSock;   //소켓 함수의 반환 값을 받는 변수
  int icSock;   //accept의 반환 값을 받는 변수
  int iRet;
  struct sockaddr_in stAddr;
  socklen_t uiSockLen=sizeof(struct sockaddr);
  char cBuff[BUFSIZ];
  const char * cP;
  pid_t pid;

  iSock = socket(AF_INET, SOCK_STREAM, 0);    //AF_INET = 2, 
  if(0 > iSock)
  {
    perror("socket : ");
    return -1;
  }
  // stAddr구조체에 socket연결을 위한 필수 정보 입력  setting
  bzero(&stAddr, sizeof(stAddr));            //구조체 비우기(0으로 채우기)
  stAddr.sin_family = AF_INET;               //#define AF_INET 2 /* IP protocol family. */
  stAddr.sin_addr.s_addr = inet_addr("127.0.0.1");    //IP와 PORT값은 헤더파일에 정의되어 있다.
  stAddr.sin_port = htons(PORTNUM);

  iRet = bind(iSock, (struct sockaddr *)&stAddr,sizeof(stAddr));
  if(iRet < 0)
  {
    perror("bind : ");
    close(iSock);

    return -2;
  }
  iRet = listen(iSock, 5);
  if(iRet != 0)
  {
    perror("listen : ");
    close(iSock);

    return -3;
  }
  while(1) 
  {
    icSock = accept(iSock, (struct sockaddr *)&stAddr, &uiSockLen); //접속자의 정보가 stAddr에 입력된다.

    if(icSock < 0)
    {
      perror("Accept : ");
      close(iSock);

      return -4;
    }
/////////////////////////////멀티 프로세스///////////////////////////////////////////
    pid = fork();

    if(pid ==  0)  //자식 프로세스이면 while문 break
    {
      break;
    }
  }
  close(iSock);

  printf("Incoming Client \n");
  //cP = inet_ntoa(stAddr.sin_addr);
  printf("Client IP :%s\n", inet_ntoa(stAddr.sin_addr));
  printf("Client Port : %d\n", ntohs(stAddr.sin_port));  

  write(icSock, "Welcome :)", sizeof("Welcome :)"));

  while(1)
  {
    read(icSock, cBuff, sizeof(cBuff));
    printf("[client]: [%s]\n", cBuff);

    write(icSock, cBuff, sizeof(cBuff));
    if(0 == strncmp("exit", cBuff, 4))
      break;

  }
  close(icSock);
  return 0;
}