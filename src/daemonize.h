
#ifndef _DEAMONIZE_H_
#define _DAEMONIZE_H_

typedef void (*daemon_function)(void *);

int daemonize(daemon_function func, void *data);

#endif

