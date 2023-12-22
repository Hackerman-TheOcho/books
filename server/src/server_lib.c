#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "server_lib.h"

#define PORT 8080

int setup_server_socket()
{
	int server_fd;
	struct sockaddr_in server_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_fd) {
		perror("Socket Error\n");
		return -1;
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&server_addr, 
				sizeof(server_addr)) == -1) {
		perror("Bind Error\n");
		close(server_fd);
		return -1;
	}

	if (listen(server_fd, 10) == -1) {
		perror("Listen Error\n");
		close(server_fd);
		return -1;
	}

	return server_fd;
}

void cleanup_server_socket(int server_fd)
{
	if (server_fd >= 0) {
		close(server_fd);
		printf("Server socket closed\n");
	}

	// TODO: More cleanup as necessary

	printf("Cleanup complete\n");
	
}