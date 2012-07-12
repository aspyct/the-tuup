#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/socket.h>
#include <strings.h>

#include "daemonize.h"

struct going_data {
	char *directory_path;
	char *socket_path;
};

void going(void *data);

int main(int argc, char *argv[]) {
	struct going_data parameters;
	parameters.directory_path = "/tmp";
	parameters.socket_path = "/tmp/going.sock";

	if (argc > 1) {
		parameters.directory_path = argv[1];
	}

	daemonize(going, &parameters);

	return EXIT_SUCCESS;
}

void going(void *data) {
	struct going_data *parameters = (struct going_data *)data;
	int server_fd;
	sockaddr_un server_address;

	if (chdir(parameters->directory_path) < 0) {
		syslog(LOG_CONS, "Could not change directory, exiting");
		exit(EXIT_FAILURE);
	}

	int fileno = open("log", O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fileno < 0) {
		syslog(LOG_CONS, "Could not open log file, exiting");
		exit(EXIT_FAILURE);
	}

	dup2(fileno, STDOUT_FILENO);
	dup2(fileno, STDERR_FILENO);

	printf("Creating server");
	unlink(parameters.socket_path);
	server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

	server_address.sun_family = AF_UNIX;
	strcopy(server_address.sun_path, 
}

