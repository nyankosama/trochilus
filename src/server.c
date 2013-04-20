/*
 * @ Copyright 2012 tomnya. All Rights Reserved.
 * Date: 2013-02-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "wrap.h"
#include "http.h"
#include "assist.h"

#define SERV_PORT 8000

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd, on;
	char str[INET_ADDRSTRLEN];
	pid_t childpid;
	

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	on = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);

	Signal(SIGCHLD, sig_child);

#ifdef DEBUG
	fprintf(stderr, "Accepting connections ...\n");
#endif

	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

#ifdef DEBUG
		printf("received from %s at PORT %d\n", 
			(char *)inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
#endif

		if ( (childpid = fork()) == 0) {
			Close(listenfd);/* 关闭子进程中不用的描述符 */
			serv_proc(connfd);
		//	http_respond(connfd);
			exit(0);
		}
		// 关闭父进程中不用的文件描述符
		Close(connfd);
	}

	Close(listenfd);
	return 0;
}

