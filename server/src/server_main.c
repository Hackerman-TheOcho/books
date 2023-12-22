#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include "server_lib.h"


int main() 
{
	int server_fd;
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	char buffer[1024];
	const char *ack_message = "Message received by seller.";

	server_fd = setup_server_socket();
	if (-1 == server_fd) {
		exit(EXIT_FAILURE);
	}

	// accept client connections
	client_addr_size = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, 
							&client_addr_size);
	if (-1 == client_fd) {
		perror("Accept Error\n");
		cleanup_server_socket(server_fd);
		exit(EXIT_FAILURE);
	}

	printf("Client connected.\n");

	// Read a message from the client
	memset(buffer, 0, sizeof(buffer));
	read(client_fd, buffer, sizeof(buffer) - 1);
	printf("Message from client: %s\n", buffer);

	// send response
	write(client_fd, ack_message, strlen(ack_message) + 1);

	// close client socket
	close(client_fd);
	printf("Client disconnected\n");


	cleanup_server_socket(server_fd);
	return 0;
}