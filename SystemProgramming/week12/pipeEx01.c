#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void){
    FILE *fp;
    int a;
    
    // "w"모드로 쓰기 전용 파이프 생성
    // 자식 프로세스는 wc -l 명령 수행
    fp = popen("wc -l","w"); //파이프 통신을 위한 FILE 포인터 획득
    if(fp==NULL){
        perror("popen failed");
        exit(1);
    }

    for(a=0;a<100;a++)
        //자식 프로세스에 연결된 파이프(fp)로 출력
        //wc -l 명령에 100줄의 문자열을 전달
        fprintf(fp,"test line\n");
    
    pclose(fp); //파이프 닫기
    return 0;
}