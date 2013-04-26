#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"

static FILE *fp;

void openlog(const char *path)
{
	fp = fopen(path, "a+");
}

/**
 * 只支持 %d 和 %s
 */
void servlog(LOG_T priority, const char *format, ...)
{
	va_list vargs;
	char current, nextchar;
	time_t timep;
	char delims[] = "\n";

	switch(priority)
	{
		case LOG_INFO:
			fprintf(fp, "[INFO]");
			break;
		case LOG_DEBUG:
			fprintf(fp, "[DEBUG]");
			break;
		case LOG_ERROR:
			fprintf(fp, "[ERROR]");
			break;
		default:
			fprintf(fp, "[DEFAULT]");
	}

	time(&timep);
	fprintf(fp, "[%s]", strtok(ctime(&timep), delims));

	va_start(vargs, format);
	while ( (current = *format++)) {
		if (current == '%'&&(nextchar=*format)!='\0') {
			format++;
			switch(nextchar) 
			{
				case 's': //%s
				{
					char *p = va_arg(vargs, char *);
					fputs(p, fp);
					break;
				}
				case 'd'://%d
				{
					int data = va_arg(vargs, int);
					char buf[16];
					sprintf(buf, "%d", data);
					fputs(buf, fp);
					break;
				}
				default:
					putc('%', fp);
					putc(nextchar, fp);
			}
		} else {
			putc(current, fp);
		}
	}
	va_end(vargs);

}

int closelog()
{
	fclose(fp);
	return 0;
}


