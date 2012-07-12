#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>

#include "daemonize.h"

struct going_data {
	char *directory_path;
	char *socket_path;
};

struct client {
	int fd;
	struct sockaddr addr;
	socklen_t addrlen;
};

void going(void *data);
void handle_client(struct client *client);

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
	struct sockaddr_un serveraddr;

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
	unlink(parameters->socket_path);
	server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, parameters->socket_path);

	int rc = bind(server_fd, (struct sockaddr*)&serveraddr, SUN_LEN(&serveraddr));
	if (rc < 0) {
		perror("Could not bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 10) < 0) {
		perror("Could not listen");
		exit(EXIT_FAILURE);
	}

	while (1) {
		struct client *client = malloc(sizeof(struct client));
		client->fd = accept(server_fd, &client->addr, &client->addrlen);

		if (client->fd < 0) {
			perror("Could not accept(), dropping client");
		}
		else {
			handle_client(client);
		}
	}
}

void handle_client(struct client *client) {
	send(client->fd, "Hello there!\n", 12, 0);
}

