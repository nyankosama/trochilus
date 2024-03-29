
#include <stdio.h>
#include <string.h>

#include "wrap.h"
#include "serv_proc.h"
#include "http.h"
#include "cgi.h"
#include "logger.h"

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

	
	memset(buf, 0, MAXLINE);
	strcpy(buf, header.locator);
	result = strtok(buf, delims);

	if (strcmp(result, "cgi-bin") == 0) {
		cgi_handle(fd, &header);
	} else {
		http_respond(fd, &header);
	}

	servlog(LOG_INFO, "[client:%s][%s] %d\n", "192.168.1.1", "GET /index.html HTTP/1.1", 200);

}

