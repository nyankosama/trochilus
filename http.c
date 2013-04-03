#include <stdio.h>
#include <string.h>
#include <string.h>
#include <unistd.h>

#include "http.h"
#include "wrap.h"

/**
 * do related handle and give request according to HTTP.
 */
void http_respond(const int fd)
{
	ssize_t n;
	char line[MAXLINE];
	char buf[MAXLINE];
	Req_header header;
	int stat;

	readline(fd, buf, MAXLINE);
#ifdef DEBUG
	fprintf(stderr, "%s", buf);
#endif
	for (; ; ) {
		n = readline(fd, line, MAXLINE);
#ifdef DEBUG
		fprintf(stderr, "%s", line);
#endif
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

	close(fd);
}


/**
 * Parse http request.
 * Store the request information into a stuct.
 * Check the file state.
 * If the file couldn't be found, return 1.
 * If the file couldn't be executed, return 0.
 * If the file could be executed, return 1.
 */
int parse_request(const char *buf, Req_header *header)
{

	return 0;
}


/**
 * Read the file and send back the content.  
 */
void file_handle(const int fd, const Req_header *header)
{

}


/**
 * execute the file and send back the output.
 */
void cgi_handle(const int fd, const Req_header *header)
{

}


/**
 * send a not found error for the client.
 */
void notfound_handle(const int fd)
{

}
