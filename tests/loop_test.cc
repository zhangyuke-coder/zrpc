
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>			/* socket类定义需要*/
#include <sys/epoll.h>			/* epoll头文件 */
#include <fcntl.h>	                /* nonblocking需要 */
#include <sys/resource.h>		/* 设置最大的连接数需要setrlimit */
#include "zrpc/net/EventLoop.h"
#include "zrpc/net/Channel.h"
#include "zrpc/net/Epoll.h"
#include <functional>
#include <iostream>
#define	MAXEPOLL	10000	/* 对于服务器来说，这个值可以很大的！ */
#define	MAXLINE		1024
#define 	PORT			20000
#define	MAXBACK	1000
 
//!> 设置非阻塞
//!> 
void readsd() {
	printf("success\n");
}
int setnonblocking( int fd )
{
	if( fcntl( fd, F_SETFL, fcntl( fd, F_GETFD, 0 )|O_NONBLOCK ) == -1 )
	{
		printf("Set blocking error : %d\n", errno);
		return -1;
	}
	return 0;
}

int main( int argc, char ** argv )
{
	int 		listen_fd;
	int 		conn_fd;
	int 		epoll_fd;
	int 		nread;
	int 		cur_fds;				//!> 当前已经存在的数量
	int 		wait_fds;				//!> epoll_wait 的返回值
	int		i;
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	struct 	epoll_event	ev;
	struct 	epoll_event	evs[MAXEPOLL];
	struct 	rlimit	rlt;		//!> 设置连接数所需
	char 	buf[MAXLINE];
	socklen_t	len = sizeof( struct sockaddr_in );
 
	//!> 设置每个进程允许打开的最大文件数
	//!> 每个主机是不一样的哦，一般服务器应该很大吧！
	//!> 
	// rlt.rlim_max = rlt.rlim_cur = MAXEPOLL;
	// if( setrlimit( RLIMIT_NOFILE, &rlt ) == -1 )	
	// {
	// 	printf("Setrlimit Error : %d\n", errno);
	// 	exit( EXIT_FAILURE );
	// }
	
	//!> server 套接口
	//!> 
	bzero( &servaddr, sizeof( servaddr ) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	servaddr.sin_port = htons( PORT );
	
	//!> 建立套接字
	if( ( listen_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		// printf("Socket Error...\n" , errno );
		exit( EXIT_FAILURE );
	}
	
	//!> 设置非阻塞模式
	//!> 
	if( setnonblocking( listen_fd ) == -1 )
	{
		printf("Setnonblocking Error : %d\n", errno);
		exit( EXIT_FAILURE );
	}
	
	//!> 绑定
	//!>
	if( bind( listen_fd, ( struct sockaddr *)&servaddr, sizeof( struct sockaddr ) ) == -1 )
	{
		printf("Bind Error : %d\n", errno);
		exit( EXIT_FAILURE );
	}
 
	//!> 监听
	//!> 
	if( listen( listen_fd, MAXBACK ) == -1 )
	{
		printf("Listen Error : %d\n", errno);
		exit( EXIT_FAILURE );
	}


	zrpc::EventLoop* loop = new zrpc::EventLoop();
	auto channel = make_shared<zrpc::Channel>(loop, listen_fd);
	channel->setEvents(EPOLLIN | EPOLLET);

	channel->setReadHandler(std::bind(readsd));
	loop->addToPoller(channel, 0);


	loop->loop();
}