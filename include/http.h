#ifndef HTTP_H
#define HTTP_H

#include "wrap.h"

enum Req_method {GET, POST, HEAD, UNIMPLEMENTED};

typedef struct req_header {
	enum Req_method method;
	char locator[MAXLINE];
	int http_type;
} Req_header;

/**
 * do related handle and give request according to HTTP.
 */
void http_respond(const int fd, const Req_header *header);
/**
 * Parse http request.
 * Store the request information into a stuct.
 */
void parse_request(const char *buf, Req_header *header);
/**
 * Read the file and send back the content.  
 */
void file_handle(const int fd, const Req_header *header);
/**
 * send a not found error for the client.
 */
void notfound_handle(const int fd);
#endif

