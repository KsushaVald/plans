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
#include <fcntl.h>

struct descriptors{
	int  fd_request;
	int  fd_answer;
};
struct msg{
	long type;
	char time[256];
	struct sockaddr_in client;
	socklen_t len;
};

void func_time(struct descriptors *des)
{	struct msg mes; long int s_time; struct tm *m_time;

	while(1){
        	msgrcv(des->fd_request,&mes, sizeof(mes),1L,0);
		mes.type=2;
		s_time=times(NULL);
		m_time=localtime(&s_time);
		strcpy(mes.time,asctime(m_time));
		msgsnd(des->fd_answer,&mes,sizeof(mes),0);
	}
}

int main()
{

	int fd_socket; struct descriptors des;
	key_t id; char buf[256];
	struct sockaddr_in my_addr;
	socklen_t  len=sizeof(struct sockaddr);
	struct msg mes; int test, err;
	pthread_t tid;

	id=ftok("server.c",'k');
	des.fd_request=msgget(id, IPC_CREAT|0666);
	id=ftok("client.c",'s');
	des.fd_answer=msgget(id,IPC_CREAT|0666);
 	my_addr.sin_family=AF_INET;
        my_addr.sin_port=htons(15242);
        my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	fd_socket=socket(AF_INET, SOCK_DGRAM,0);
	fcntl(fd_socket,test,O_NONBLOCK);
	bind(fd_socket,(struct sockaddr*)&my_addr,sizeof(my_addr));
	pthread_create(&tid,NULL,(void*)&func_time,(void*)&des);

	while(1){
		if(recvfrom(fd_socket,buf,256,0,(struct sockaddr*)&mes.client,&len)==-1){
			err=errno;
			if(err!=EWOULDBLOCK){
				perror("recvfrom");
				exit(-1);
			}
		}
		else{
			mes.type=1L;
			mes.len=len;
			msgsnd(des.fd_request, &mes,sizeof(mes), 0);
		}
		test=msgrcv(des.fd_answer,&mes, sizeof(mes),2L,0);
		if(test<0){
			err=errno;
			if(err==ENOMSG){
				continue;
			}
			perror("msgrcv");
			return 1;
		}
		strcpy(buf,mes.time);
 		sendto(fd_socket, buf, 256,0,(struct sockaddr*)&mes.client,mes.len);
	}
}

