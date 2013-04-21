#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "wrap.h"
#include "http.h"

#define LONG_LINE 1024

void cgi_handle(const int fd, const Req_header *header)
{
	char line[MAXLINE];

	//设置环境变量
	for (; ; ) {
		readline(fd, line, MAXLINE);
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

	memset(line, 0, MAXLINE);
	readn(fd, line, 13);
#ifdef DEBUG
	fprintf(stderr, "%s", line);
#endif

	Close(fd);
}

void head_handle(const int fd, const Req_header *header)
{

}
