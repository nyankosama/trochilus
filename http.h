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
void http_respond(const int fd);
/**
 * Parse http request.
 * Store the request information into a stuct.
 * Check the file state.
 * If the file couldn't be found, return 1.
 * If the file couldn't be executed, return 0.
 * If the file could be executed, return 1.
 */
int parse_request(const char *buf, Req_header *header);
/**
 * Read the file and send back the content.  
 */
void file_handle(const int fd, const Req_header *header);
/**
 * execute the file and send back the output.
 */
void cgi_handle(const int fd, const Req_header *header);
/**
 * send a not found error for the client.
 */
void notfound_handle(const int fd);
#endif

