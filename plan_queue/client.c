#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <pthread.h>


void request(int *fd){

	char mes[5]="time\0"; char buf[256];
	send(*fd,mes,20,0);
	recv(*fd,buf,256,0);
	printf("%s", buf);

}

int main(){

	int fd, i;
	struct sockaddr_in my_addr;
	socklen_t len;
	pthread_t tid[10];

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(15242);
	my_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	len=sizeof(my_addr);

        fd=socket(AF_INET, SOCK_DGRAM,0);
	if(connect(fd,(struct sockaddr*)&my_addr, len)==-1){
		perror("connect");
		exit(-1);
	}
	for(i=0; i<10; i++){
		pthread_create(&tid[i],NULL,(void*)&request,(void*)&fd);
	}
	for(i=0; i<10; i++){
		pthread_join(tid[i], NULL);
	}

}
