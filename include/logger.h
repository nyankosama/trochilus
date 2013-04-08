#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

void servlog(int priority, const char *format, ...);

void vservlog(int priority, const char *format, va_list ap);

#endif

