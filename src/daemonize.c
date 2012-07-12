#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

#include "daemonize.h"

int daemonize(daemon_function func, void *data) {
	pid_t pid;
	pid_t sid;

	pid = fork();
	if (pid < 0) {
		perror("Could not fork");
		return -1;
	}

	if (!pid) {
		umask(0);

		sid = setsid();
		if (sid < 0) {
			exit(EXIT_FAILURE);
		}
	
		if (chdir("/") < 0) {
			exit(EXIT_FAILURE);
		}
	
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	
		func(data);
	}

	return pid;
}

