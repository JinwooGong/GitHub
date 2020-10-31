#include <stdio.h>
#include <netdb.h>

int main(void){
	struct servent *port; // servent 구조체
	int n;

	setservent(0);
	
	while((port=getservent())!=NULL){
		printf("Name=%s, Port=%d\n",port->s_name,port->s_port);
	}
	
	endservent();
	
	return 0;
}
