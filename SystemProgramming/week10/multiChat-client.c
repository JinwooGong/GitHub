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
  struct sockaddr_in stAddr;
  int iSock;
  int iRet;
  char cBuff[BUFSIZ];
  socklen_t uiSockLen=sizeof(struct sockaddr);
  pid_t pid;

  iSock = socket(AF_INET, SOCK_STREAM, 0);
  if(0 > iSock)
  {
    perror("socket : ");
    return -1;
  }

  bzero(&stAddr, sizeof(stAddr));
  stAddr.sin_family = AF_INET;

  stAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  stAddr.sin_port = htons(PORTNUM);

  iRet = connect(iSock, (struct sockaddr *)&stAddr, uiSockLen);
  if(0 > iRet)
  {
    perror("connect : ");
    return -2;
  }

  pid = fork();

  if(pid > 0)
  {
    close(0);
    while(1)
    {
      read(iSock, cBuff, sizeof(cBuff));
      printf("[server]: [%s]\n", cBuff);

      if(0 == strncmp("exit", cBuff, 4))
        break;
    }

  }
  else
  {
    close(1);
    while(1)
    {
      iRet = read(0, cBuff, sizeof(cBuff));
      cBuff[iRet-1] = 0;
      write(iSock, cBuff, sizeof(cBuff));

    }

  }

  close(iSock);
  return 0;
}