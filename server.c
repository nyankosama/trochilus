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
#include <fcntl.h>

#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 8000

void str_echo(int sockfd);
void sig_child(int signo);
void sendfile(const char *path,int connfd);
void outcgi(const char *path, int connfd);
void respond(int connfd);

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char str[INET_ADDRSTRLEN];
	int i, n;
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
			respond(connfd);
//			str_echo(connfd);
			exit(0);
		}
	}
	
	Close(connfd);
	return 0;
}

void respond(int connfd)
{
	ssize_t n;
	char line[MAXLINE];
	char *token, *path;
	struct stat stbuf;
	for (; ; ) {
		if ( (n = readline(connfd, line, MAXLINE)) == 0) {
			return;
		}
		token = strtok(line, " ");
		if (token != NULL && strcmp(token, "GET") == 0) {
			token = strtok(NULL, " ");
			token = strtok(token, "/");
			if (token == NULL) {
				printf("index.html\n");
				path = "index.html";
			} else {
				printf("%s\n", token);
				path = token;
			}
			if (stat(path, &stbuf) < 0){
				fputs("error!!!!", stderr);
				continue;
			}
			if (stbuf.st_mode & S_IEXEC) {
				outcgi(path, connfd);
				printf("cgi\n");
			} else {
				sendfile(path, connfd);
				printf("file\n");
			}
		}
	}
}

void sendfile(const char *path,int connfd){
	int fd;
	char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html></html>\n";
	writen(connfd, response, strlen(response));
}

void outcgi(const char *path, int connfd){

}

void sig_child(int signo)
{
	pid_t pid;
	int stat;
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}