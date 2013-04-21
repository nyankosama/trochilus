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
 * Parse http request.
 * Store the request information into header.
 */
void parse_request(const char *buf, Req_header *header)
{
	char method[10];
	char httptype[20];
	char url[255];

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
	sprintf(header->locator, "%s", url);

}

/**
 * do related handle and give request according to HTTP.
 */
void http_respond(const int fd, const Req_header *header)
{
	char line[MAXLINE];

	for (; ; ) {
		readline(fd, line, MAXLINE);
#ifdef DEBUG
		fprintf(stderr, "%s", line);
#endif
		//checks http header end
		if (strcmp(line, "\r\n") == 0) {
			break;
		}
	}

#ifdef DEBUG
	printf("end of request header\n");
#endif

	file_handle(fd, header);

	Close(fd);
}


/**
 * Read the file and send back the content.  
 */
void file_handle(const int fd, const Req_header *header)
{
	FILE *fp = NULL;
	char url[255];
	sprintf(url, "%s%s", ROOTDIR, header->locator);
	
	fp = fopen(url, "r");
	if(fp == NULL){
		//未找到文件
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
