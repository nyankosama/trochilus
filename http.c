#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "http.h"
#include "wrap.h"

/**
 * do related handle and give request according to HTTP.
 */
void http_respond(int fd)
{
	ssize_t n;
	char line[MAXLINE];
	Req_header header;
	readline(fd, line, MAXLINE);
	
	
	
	for (; ; ) {

	}

	close(fd);

}


/**
 * Parse http request.
 * Store the request information into a stuct.
 * Determine whether the file can be found.
 * Determine whether the file is executable.
 */
void parse_request(int fd)
{

}


/**
 * Read the file and send back the content.  
 */
void file_handle(int fd)
{

}


/**
 * execute the file and send back the output.
 */
void cgi_handle(int fd)
{

}


/**
 * send a not found error for the client.
 */
void notfound_handle(int fd){

}
