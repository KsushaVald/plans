#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <time.h>

void new_port(int *port){
	int fd_socket, fd_client;
	struct  sockaddr_in my_addr, client;
	socklen_t len=sizeof(struct sockaddr);
	long int s_time; struct tm *m_time; char time[256];

	my_addr.sin_family=AF_INET;
        my_addr.sin_port=htons(*port);
        my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        fd_socket=socket(AF_INET,SOCK_STREAM,0);
        if(bind(fd_socket,(struct sockaddr*)&my_addr,sizeof(my_addr))==-1){
		perror("bind");
	}
        listen(fd_socket,5);

	fd_client=accept(fd_socket,(struct sockaddr*)&client,&len);
	s_time=times(NULL);
        m_time=localtime(&s_time);
	strcpy(time,asctime(m_time));
	send(fd_client,&time,sizeof(time),0);

}


int main(){

	int fd_socket, fd_client;
	struct sockaddr_in my_addr, client;
	int port=15242;
	pthread_t tid;
	socklen_t len=sizeof(struct sockaddr);

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	fd_socket=socket(AF_INET,SOCK_STREAM,0);
	bind(fd_socket,(struct sockaddr*)&my_addr,sizeof(my_addr));
	listen(fd_socket,5);

	while(1){
		fd_client=accept(fd_socket,(struct sockaddr*)&client, &len);
		port++;
		pthread_create(&tid, NULL,(void*)&new_port,(void*)&port);
		send(fd_client,(void*)&port,sizeof(int),0);
		close(fd_client);
	}
}
