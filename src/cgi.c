#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cgi.h"
#include "wrap.h"
#include "http.h"

#define LONGLINE 1024
#define CGIDIR "../cgi-bin/"

void set_metavar(char line[]);
int get_cginame(const Req_header *header, char cginame[]);

void cgi_handle(const int fd, const Req_header *header)
{
	char line[MAXLINE];

	for (; ; ) {
		memset(line, 0, MAXLINE);
		readline(fd, line, MAXLINE);
		//设置环境变量
		set_metavar(line);
#ifdef DEBUG
		fprintf(stderr, "%s", line);
#endif
		//检查http头是否结束
		if (strcmp(line, "\r\n")==0 || strcmp(line, "\n")==0) {
			break;
		}
	}
#ifdef DEBUG
	fprintf(stderr, "end of request header\n");
#endif

	if (header->method == GET) {
		get_handle(fd, header);
	} else if (header->method == POST) {
		post_handle(fd, header);
	} else if (header->method == HEAD) {
		head_handle(fd, header);
	} 
}

void get_handle(const int fd, const Req_header *header)
{
	extern char **environ;

	char buf[MAXLINE], *emptylist[] = {NULL};
	pid_t pid;
	char cgi_name[MAXLINE];

	sprintf(buf, "HTTP/1.1 200 OK\r\nServer:trochilus\r\n");
	writen(fd, buf, strlen(buf));

	if ( (pid = fork()) == 0) {
		if (get_cginame(header, cgi_name) != 0) {
			return;
		}

		dup2(fd, STDOUT_FILENO);
		execve(cgi_name, emptylist, environ);
		Close(fd);
	} else {
		waitpid(pid, NULL, 0);
		Close(fd);
	}
}

void post_handle(const int fd, const Req_header *header)
{
	extern char **environ;

	char buf[MAXLINE], *emptylist[] = {NULL};
	pid_t pid;
	char cgi_name[MAXLINE];

	char post_var[LONGLINE];
	int length;
	int filedes[2];
	pipe(filedes);

	length = atoi(getenv("CONTENT_LENGTH"));
	readn(fd, post_var, length);
	writen(filedes[1], post_var, length);

	sprintf(buf, "HTTP/1.1 200 OK\r\nServer:trochilus\r\n");
	writen(fd, buf, strlen(buf));

	if ( (pid = fork()) == 0) {
		Close(filedes[1]);
		if (get_cginame(header, cgi_name) != 0) {
			return;
		}

		dup2(filedes[0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		execve(cgi_name, emptylist, environ);
		Close(fd);
	} else {
		Close(filedes[1]);
		Close(filedes[0]);
		waitpid(pid, NULL, 0);
		Close(fd);
	}
}

void head_handle(const int fd, const Req_header *header)
{

}

void set_metavar(char line[])
{
	char delims[] = ":";
	char *result = NULL;

	result = strtok(line, delims);
	if (strcmp(result, "Host") == 0) {
		result = strtok(NULL, delims);
		setenv("SERVER_NAME", result, 1);
		if ( (result = strtok(NULL, delims)) != NULL) {
			setenv("SERVER_PORT", result, 1);
		} else {
			setenv("SERVER_PORT", "80", 1);
		}
	} else if (strcmp(result, "Accept") == 0) {
		setenv("HTTP_ACCEPT", strtok(NULL, delims), 1);
	} else if (strcmp(result, "User-Agent") == 0) {
		setenv("HTTP_USER_AGENT", strtok(NULL, delims), 1);
	} else if (strcmp(result, "Accept-Encoding") == 0) {
		setenv("HTTP_ACCEPT_ENCODING", strtok(NULL, delims), 1);
	} else if (strcmp(result, "Accept-Languange") == 0) {
		setenv("HTTP_ACCEPT_LANGUAGE", strtok(NULL, delims), 1);
	} else if (strcmp(result, "Accept-Charset") == 0) {
		setenv("HTTP_ACCEPT_CHARSET", strtok(NULL, delims), 1);
	} else if (strcmp(result, "Connection") == 0) {
		setenv("HTTP_CONNECTION", strtok(NULL, delims), 1);
	} else if (strcmp(result, "Content-Length") == 0) {
		setenv("CONTENT_LENGTH", strtok(NULL, delims), 1);
	}
}


int get_cginame(const Req_header *header, char cgi_name[])
{
	char buf[MAXLINE];
	char *result = NULL;
	char delims[] = "/";

	strcpy(buf, header->locator);
	strtok(buf, delims);
	if ( (result = strtok(NULL, delims)) == NULL) {
		return 1;
	} else {
		sprintf(cgi_name, "%s%s", CGIDIR, result);
		return 0;
	}

}
