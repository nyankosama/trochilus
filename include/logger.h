#ifndef LOGGER_H
#define LOGGER_H

typedef enum LOG_TYPE {
	LOG_INFO,
	LOG_DEBUG,
	LOG_ERROR
} LOG_T;

void servlog(LOG_T priority, const char *format, ...);

void openlog(const char *path);

int closelog();

#endif

