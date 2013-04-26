#include <stdio.h>
#include <string.h>

#include "wrap.h"
#include "config.h"

int readconfig(const char *path)
{
	char option[MAXLINE], value[MAXLINE], buf[MAXLINE];
	char delims[] = "=";
	char delims1[] = "\n";

	FILE *fp = NULL;

	if ( (fp=fopen(path, "r")) == NULL) {
		return 1;
	}

	while (fgets(buf, MAXLINE, fp) != NULL) {
		strcpy(option, strtok(buf, delims));
		strcpy(value, strtok(NULL, delims));
		strcpy(value, strtok(value, delims1));
		setenv(option, value, 1);
	}

	return 0;
}

char* getconfig(const char *name)
{
	return getenv(name);
}
