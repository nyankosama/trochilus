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

#define MAXLINE 80
#define SERV_PORT 8000

void str_echo(int sockfd);
void sig_child(int signo);

/**
 * Parse http request.
 * Store the request information into a stuct.
 * Determine whether the file can be found.
 * Determine whether the file is executable.
 */
void parse_request(int fd);
/**
 * Read the file and send back the content.  
 */
void file_handle(int fd);
/**
 * execute the file and send back the output.
 */
void cgi_handle(int fd);
/**
 * send a not found error for the client.
 */
void notfound_handle(int fd);

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char str[INET_ADDRSTRLEN];
	pid_t childpid;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);

	Signal(SIGCHLD, sig_child);

	printf("Accepting connections ...\n");

	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		if ( (childpid = fork()) == 0) {
			Close(listenfd);/* 关闭子进程中不用的描述符 */
			printf("received from %s at PORT %d\n", 
				(char *)inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
				ntohs(cliaddr.sin_port));
			str_echo(connfd);
			exit(0);
		}
	}
	
	Close(connfd);
	return 0;
}

void str_echo(int connfd)
{
	ssize_t n;
	char line[MAXLINE];
	for (; ; ) {
		if ( (n = readline(connfd, line, MAXLINE)) == 0) {
			printf("%s\n", line);
			return;
		}
		printf("%s", line);
		writen(connfd, line, strlen(line));
	}
}


void sig_child(int signo)
{
	pid_t pid;
	int stat;
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}
