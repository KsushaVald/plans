#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <pthread.h>


void clients(){

        int fd_socket; int port, test; char buf[256];
        struct sockaddr_in my_addr;
        my_addr.sin_family=AF_INET;
        my_addr.sin_port=htons(15242);
        my_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        fd_socket=socket(AF_INET,SOCK_STREAM,0);
        connect(fd_socket,(struct sockaddr*)&my_addr, sizeof(my_addr));
        recv(fd_socket,&port,sizeof(int),0);
        my_addr.sin_port=htons(port);
        close(fd_socket);
	//sleep(1);
        fd_socket=socket(AF_INET,SOCK_STREAM,0);
        connect(fd_socket,(struct sockaddr*)&my_addr, sizeof(my_addr));
        if(test==-1){
                perror("connect");
                exit(-1);
        }
        recv(fd_socket,&buf,256, 0);
        printf("%s",buf);

}

int main(){

	 pthread_t tid; int i;
	// for(i=0; i<1; i++){
		pthread_create(&tid,NULL,(void*)&clients,NULL);
//	 }
//	 for(i=0; i<10; i++){
		pthread_join(tid,NULL);
//	 }
}
