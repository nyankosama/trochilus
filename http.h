#ifndef HTTP_H
#define HTTP_H

#include "wrap.h"


enum Req_method {GET, HEAD, UNIMPLEMENTED};

typedef struct req_header {
	enum Req_method method;
	char locator[MAXLINE];
	int http_type;
} Req_header;

/**
 * do related handle and give request according to HTTP.
 */
void http_respond(int fd);
/**
 * Parse http request.
 * Store the request information into a stuct.
 * Determine whether the file can be found.
 * Determine whether the file is executable.
 */
void parse_request(int fd);
/**
 * Read the file and send back the content.  
 */
void file_handle(int fd);
/**
 * execute the file and send back the output.
 */
void cgi_handle(int fd);
/**
 * send a not found error for the client.
 */
void notfound_handle(int fd);
#endif

