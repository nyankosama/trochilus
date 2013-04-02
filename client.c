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
#include <arpa/inet.h>

#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 8000

void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr;
	int sockfd;

	if (argc != 2) {
		fputs("usage: tcpcli <IPaddress>\n", stderr);
		exit(1);
	}

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);

	Close(sockfd);
	return 0;
}

void str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		writen(sockfd, sendline, strlen(sendline));
		if (readline(sockfd, recvline, MAXLINE) == 0) {
			fputs("str_cli:server terminated prematurely", stderr);
		}
		printf("client:%s", recvline);
	}
}
