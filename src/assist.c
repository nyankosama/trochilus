#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "wrap.h"

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

#ifdef DEBUG
	printf("child %d terminated\n", pid);
#endif

	return;
}
