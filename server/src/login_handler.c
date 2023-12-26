#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "login_handler.h"

#define USERNAME_MAX_LEN 25
#define PASSWORD_MAX_LEN 25
#define BUFFER_SIZE 1024


static bool verify_credentials(ht_t *user_table, const char *username, 
			       const char *password)
{
	// look up username in hash table
	char *stored_password = ht_search(user_table, username);
	if (NULL == stored_password) {
		// user not found
		return false;
	}

	// compare provided password with stored password & return true
	return strcmp(stored_password, password) == 0;
}

static void send_generic_response(int client_fd, const char *response) 
{
	if (response != NULL) {
		// Send the response to the client
		write(client_fd, response, strlen(response) + 1);
	}
}

void handle_client_request(int client_fd, ht_t *user_table)
{
	char username[USERNAME_MAX_LEN];
	char password[PASSWORD_MAX_LEN];
	request_type_t request_type;

	// receive login request
	if (!read_client_data(client_fd, &request_type, username, 
				sizeof(username), password, 
				sizeof(password))) {
		send_login_response(client_fd, false);
		return;
	}

	// Process based on request type
	switch(request_type) {
		case REQUEST_LOGIN:
		process_login_request(client_fd, user_table, username, password);
		break;
		case REQUEST_CREATE:
		if (create_user(user_table, username, password)) {
			send_generic_response(client_fd, "Account Creation Successful");
		} else {
			send_generic_response(client_fd, "Account Creation Failed");
		}
		break;
		// ... handle other request types ...
	}	
}

void process_login_request(int client_fd, ht_t *user_table, 
				const char *username, const char *password) 
{
	// Check credentials
	bool login_success = verify_credentials(user_table, username, password);

	// Send login response to client
	send_login_response(client_fd, login_success);
}

bool create_user(ht_t *user_table, const char *username, const char *password)
{
	// check for existing user entry
	if (ht_search(user_table, username) != NULL) {
		return false;  // existing user
	}

	// dup password for storage
	char *password_copy = strdup(password);
	if (NULL == password_copy) {
		return false;
	}

	// TODO: hash password
	// char *hashed_password = hash_password(password_copy);

	// add user to hash table
	if (!ht_insert(user_table, username, password_copy)) {
		free(password_copy);
		return false;
	}

	save_users(user_table, USER_FILE);
	return true;
}

bool read_client_data(int client_fd, request_type_t *request_type, 
			char *username, size_t username_size, char *password, 
			size_t password_size)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read = 0;

	// init buffer & read data from client
	memset(buffer, 0, sizeof(buffer));
	bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0) {
		return false;
	}

	// parse buffer & extract request type, username, password
	char *token;
	char *rest = buffer;

	// get request type
	token = strtok_r(rest, ":", &rest);
	if (NULL == token) {
		return false;
	}
	if (strcmp(token, "login") == 0) {
		*request_type = REQUEST_LOGIN;
	} else if (strcmp(token, "create_account") == 0) {
		*request_type = REQUEST_CREATE;
	}

	// get username
	token = strtok_r(NULL, ":", &rest);
	if (NULL == token) {
		return false;
	}
	strncpy(username, token, username_size - 1);
	username[username_size - 1] = '\0';

	// get password
	token = strtok_r(NULL, ":", &rest);
	if (NULL == token) {
		return false;
	}
	strncpy(password, token, password_size - 1);
	password[password_size - 1] = '\0';

	return true;
}

void send_login_response(int client_fd, bool login_success)
{
	// format and send a response message
	const char *response = login_success ? "Login successful" : 
					       "Login failed";
	
	write(client_fd, response, strlen(response) + 1); // +1 for null term
}
