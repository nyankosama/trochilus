
#include <stdio.h>
#include <string.h>

#include "wrap.h"
#include "serv_proc.h"
#include "http.h"
#include "cgi.h"

void serv_proc(int fd){
	char buf[MAXLINE];
	Req_header header;

	char delims[] = "/";
	char *result = NULL;

	readline(fd, buf, MAXLINE);
#ifdef DEBUG
	fprintf(stderr, "%s", buf);
#endif

	parse_request(buf, &header);

	result = strtok(header.locator, delims);

	if (strcmp(result, "cgi-bin") == 0) {
		cgi_handle(fd, &header);
	} else {
		http_respond(fd, &header);
	}

}

