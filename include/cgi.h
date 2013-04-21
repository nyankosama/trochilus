#ifndef CGI_H
#define CGI_H

#include "http.h"

void cgi_handle(const int fd, const Req_header *header);

void get_handle(const int fd, const Req_header *header);

void post_handle(const int fd, const Req_header *header);

void head_handle(const int fd, const Req_header *header);


#endif
