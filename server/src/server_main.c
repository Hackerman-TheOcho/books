#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include "server_lib.h"
#include "login_handler.h"
#include "ht.h"

uint64_t hash(const char *name, size_t length) 
{
	uint64_t hash_val = 0;
	for (size_t i = 0; i < length; i++) {
		hash_val += name[i];
		hash_val = (hash_val * name[i]);
	}
	return hash_val;
}

int main() 
{
	// init socket stuff
	int server_fd;
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_addr_size;

	// init user hash table
	ht_t *user_table = ht_create(HT_SIZE, hash, NULL);
	if (NULL == user_table) {
		perror("Failed to create hash table\n");
		return 1;
	}

	// load users from file
	load_users(user_table, USER_FILE);

	// setup server
	server_fd = setup_server_socket();
	if (-1 == server_fd) {
		ht_destroy(user_table);
		exit(EXIT_FAILURE);
	}

	while (1) {
		// accept client connections
		client_addr_size = sizeof(client_addr);
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, 
					&client_addr_size);
		if (-1 == client_fd) {
			perror("Accept error");
			continue;
		}

		printf("Client connected.\n");

		// handle client account related requests requests
		handle_client_request(client_fd, user_table);

		// close client socket
		close(client_fd);
		printf("Client disconnected.\n");
	}

	// before exiting
	save_users(user_table, USER_FILE);
	ht_destroy(user_table);
	cleanup_server_socket(server_fd);

	return 0;
}



	// // test
	// int server_fd;
	// int client_fd;
	// struct sockaddr_in client_addr;
	// socklen_t client_addr_size;
	// char buffer[1024];
	// const char *ack_message = "Message received by server.";

	// server_fd = setup_server_socket();
	// if (-1 == server_fd) {
	// 	exit(EXIT_FAILURE);
	// }

	// // accept client connections
	// client_addr_size = sizeof(client_addr);
	// client_fd = accept(server_fd, (struct sockaddr *)&client_addr, 
	// 						&client_addr_size);
	// if (-1 == client_fd) {
	// 	perror("Accept Error\n");
	// 	cleanup_server_socket(server_fd);
	// 	exit(EXIT_FAILURE);
	// }

	// printf("Client connected.\n");

	// // Read a message from the client
	// memset(buffer, 0, sizeof(buffer));
	// read(client_fd, buffer, sizeof(buffer) - 1);
	// printf("Message from client: %s\n", buffer);

	// // send response
	// write(client_fd, ack_message, strlen(ack_message) + 1);

	// // close client socket
	// close(client_fd);
	// printf("Client disconnected\n");

	// cleanup_server_socket(server_fd);