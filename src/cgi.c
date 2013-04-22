#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "wrap.h"
#include "http.h"

#define LONGLINE 1024
#define CGIDIR "../cgi-bin/"

void set_metavar(char line[]);

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

}

void post_handle(const int fd, const Req_header *header)
{

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
	} else if (strcmp(result, "Connection") == 0) {

	}
}


