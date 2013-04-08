#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "http.h"
#include "wrap.h"

#define ROOTDIR "../htdocs"

int get_file_type(const char *filename);

static const char content_types[][15] = {
	"text/html", "image/png"
};
/**
 * do related handle and give request according to HTTP.
 */
void http_respond(const int fd)
{
	char line[MAXLINE];
	char buf[MAXLINE];
	Req_header header;
	int stat;

	readline(fd, buf, MAXLINE);
#ifdef DEBUG
	fprintf(stderr, "%s", buf);
#endif

	for (; ; ) {
		readline(fd, line, MAXLINE);
		//checks http header end
		if (strcmp(line, "\r\n") == 0) {
			break;
		}
	}

#ifdef DEBUG
	printf("end of request\n");
#endif

	if ( ( stat = parse_request(buf, &header)) == -1) {
		notfound_handle(fd);
	} else if (stat == 0) {
		file_handle(fd, &header);
	} else if (stat == 1){
		cgi_handle(fd, &header);
	}

	Close(fd);
}


/**
 * Parse http request.
 * Store the request information into a stuct.
 * Check the file state.
 * If the file couldn't be found, return -1.
 * If the file couldn't be executed, return 0.
 * If the file could be executed, return 1.
 */
int parse_request(const char *buf, Req_header *header)
{
	char method[10];
	char httptype[20];
	char url[255];

	struct stat st;

	sscanf(buf, "%s%s%s", method, url, httptype);

	//设置header的http_type
	sscanf(httptype, "HTTP/1.%d", &header->http_type);

	//设置header的method
	if(strcasecmp(method, "GET")==0)
		header->method = GET;
	else if(strcasecmp(method, "POST")==0)
		header->method = POST;
	else
		header->method = UNIMPLEMENTED;

	//设置header的locator
	sprintf(header->locator, "%s%s", ROOTDIR, url);

	//获取文件信息，判断返回值
	if ( stat( header->locator , &st) == -1){
		//未找到文件
		return -1;
	} else if( ( st.st_mode & S_IXOTH) == S_IXOTH){
		return 1;
	} else {
		return 0;
	}
}


/**
 * Read the file and send back the content.  
 */
void file_handle(const int fd, const Req_header *header)
{
	FILE *fp = NULL;
	
	fp = fopen(header->locator , "r");
	if(fp == NULL){
		notfound_handle(fd);
	}else{
		char buf[1024]="\0";
		char response_head[] = "HTTP/1.1 200 OK\r\n"
					"Server: trochilus\r\n"
					"Content-Type: %s\r\n"
					"\r\n";
		char filebuf[1024];
		int filetype = get_file_type(header->locator);
		int ffd = fileno(fp);
		int readnum = 0;

		sprintf(buf,response_head, content_types[filetype]);
		writen(fd,buf,strlen(buf));
		while( (readnum = read(ffd,filebuf,sizeof(filebuf))) > 0 )
			write(fd,filebuf,readnum);
		strcpy(filebuf , "\r\n");
		writen(fd,filebuf,strlen(filebuf));
		fclose(fp);
	}
}


/**
 */
void cgi_handle(const int fd, const Req_header *header)
{
	dup2(fd, STDOUT_FILENO);
	execl("/bin/sh", "sh", header->locator, NULL);
}


/**
 * send a not found error for the client.
 */
void notfound_handle(const int fd)
{
	char resp_header[] = "HTTP/1.1 404 Not Found\r\n"
						"Content-Type: text/html\r\n\r\n";
	char resp[200];
	sprintf(resp, "%s%s", resp_header, "<html><body>404 Not found</body></html>\r\n");
	writen(fd, resp, strlen(resp));
}


int get_file_type(const char *filename)
{
	char *p = strrchr(filename, '.');
	if(strcmp(p, ".png")==0)
		return 1;
	else 
		return 0;
}
