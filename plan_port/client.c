#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

void clients(){

        int fd_socket; int port, test; char buf[256];
        struct sockaddr_in my_addr; fd_set set, rset, wset;
	struct timeval t;
	t.tv_sec=3;
	t.tv_usec=0;
        my_addr.sin_family=AF_INET;
        my_addr.sin_port=htons(15242);
        my_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        fd_socket=socket(AF_INET,SOCK_STREAM,0);
	connect(fd_socket,(struct sockaddr*)&my_addr, sizeof(my_addr));
	if(test==-1){
                perror("connect");
                exit(-1);
        }
        recv(fd_socket,&port,sizeof(int),0);
        my_addr.sin_port=htons(port);
        close(fd_socket);
        fd_socket=socket(AF_INET,SOCK_STREAM,0);
       	FD_ZERO(&set);
	FD_SET(fd_socket,&set);
	test=select(fd_socket+1,NULL,&set,NULL,NULL);
	if(test==-1){
		perror("select");
	}
	test=connect(fd_socket,(struct sockaddr*)&my_addr, sizeof(my_addr));
	if(test==-1){
                perror("connect");
              //  exit(-1);
        }
        recv(fd_socket,&buf,256, 0);
        printf("%s",buf);

}

int main(){

	 pthread_t tid[10]; int i;
	 for(i=0; i<10; i++){
		pthread_create(&tid[i],NULL,(void*)&clients,NULL);
	 }
	 for(i=0; i<10; i++){
		pthread_join(tid[i],NULL);
	 }
}
