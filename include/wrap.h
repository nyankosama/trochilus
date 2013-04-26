#ifndef WRAP_H
#define WRAP_H

#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

#define MAXLINE 80
#define LONGLINE 1024

typedef void Sigfunc(int);

void perr_exit(const char *s);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
int Bind(int fd, const struct sockaddr *sa, socklen_t salenptr);
int Connect(int fd, const struct sockaddr *sa, socklen_t addrlen);
void Listen(int fd, int backlog);
int Socket(int family, int type, int protocol);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, const void *ptr, size_t nbytes);
void Close(int fd);

ssize_t readn(int fd, void *vptr, size_t n);
ssize_t writen(int fd, const void *vptr, size_t n);
ssize_t readline(int fd, void *ptr, size_t maxlen);

Sigfunc *Signal(int signo, Sigfunc *fun);

#endif
