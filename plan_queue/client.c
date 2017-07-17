#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>


int main(){

	int fd; char mes[20]; char buf[20];
        struct sockaddr_in my_addr;
	socklen_t len;

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(15242);
	my_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	len=sizeof(my_addr);

        fd=socket(AF_INET, SOCK_DGRAM,0);
	if(connect(fd,(struct sockaddr*)&my_addr, len)==-1){
		perror("connect");
		exit(-1);
	}
	scanf("%s", &mes);
	send(fd,mes,20,0);
	recv(fd,buf,20,0);
	printf("%s\n", buf);
}
