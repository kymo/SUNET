#include<stdio.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<assert.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENT_NUM 1024
#define BUFFER_SIZE 4098

void setnonblocking(int fd)
{
	fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

void addfd(int epollfd, int fd, bool enable)
{
	epoll_event event;
	event.data.fd=fd;
	event.events=EPOLLIN;
	if(enable)
	{
		event.events |= EPOLLET;
	}
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
	setnonblocking(fd);
}

void lt(epoll_event* event, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	for(int i=0;i<number;i++)
	{
		int sockfd=event[i].data.fd;
		if(sockfd==listenfd)
		{
			struct sockaddr_in client_address;
			socklen_t client_addrlength=sizeof(client_address);
			int confd=accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
			addfd(epollfd,confd, false);
		}
		else if(event[i].events & EPOLLIN)//?????????????
		{
			printf("event trigger once\n");
			memset(buf,'\0',BUFFER_SIZE);
			int ret=recv(sockfd,buf,BUFFER_SIZE-1,MSG_WAITALL);
			if(ret<=0)
			{
				close(sockfd);
				continue ;
			}
			printf("get %d bytes of content:%s\n",ret,buf);
		}
		else
		{
			printf("something wrong\n");
		}
	}
}

void et(epoll_event* event, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	for(int i=0;i<number;i++)
	{
		int sockfd=event[i].data.fd;
		if(sockfd==listenfd)
		{
			struct sockaddr_in client_address;
			socklen_t client_length=sizeof(client_address);
			int confd=accept(listenfd,(struct sockaddr*)&client_address,&client_length);//尤其注意accept的最后一个参数
			addfd(epollfd, confd, true);
		}
		else if(event[i].events & EPOLLIN)
		{
			printf("event trigger once\n");
			while(true)
			{
				memset(buf,'\0',BUFFER_SIZE);
				int ret=recv(sockfd,buf,BUFFER_SIZE-1,0);
				if(ret<0)
				{
					if((errno==EAGAIN)||(errno==EWOULDBLOCK))
					{
						printf("read later");
						break;
					}
					close(sockfd);
					break ;
				}
				else if(ret==0)
				{
					close(sockfd);
				}
				else
				{
					printf("get %d bytes of content:%s\n",ret,buf);
				}
			}

		}
		else
		{
			printf("something wrong\n");
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc<=2)
	{
		printf("usage:%s ip_address port_number\n",basename(argv[0]));
		return 1;
	}
	int ret;
	const char *ip=argv[1];
	int port=atoi(argv[2]);
	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);//点分十进制转二进制
	address.sin_port=htons(port);//主机字序转网络字序

	int listenfd=socket(AF_INET, SOCK_STREAM,0);
	assert(listenfd>=0);
	ret=bind(listenfd,(struct sockaddr*)&address, sizeof(address));
	assert(ret!=-1);
	ret=listen(listenfd,5);
	assert(ret!=-1);

	epoll_event event[MAX_EVENT_NUM];
	int epollfd=epoll_create(5);
	assert(epollfd!=-1);
	addfd(epollfd,listenfd,true);

	while(1)
	{
		ret=epoll_wait(epollfd,event,MAX_EVENT_NUM,-1);//-1：无超时时间，永久等待
		if(ret<0)
		{
			printf("epoll fail\n");
			break;
		}
		//lt(event,ret,epollfd,listenfd);                         /*使用LT模式*/
		et(event,ret,epollfd,listenfd);                       /*使用ET模式*/
	}
	close(listenfd);
	return 0;
}
