#ifndef _LOGIN_H
#define _LOGIN_H

#include "ht.h"
#include "file_io.h"

#define HT_SIZE 100
#define USER_FILE "users.txt"

typedef enum {
	REQUEST_LOGIN,
	REQUEST_CREATE,
} request_type_t;

void handle_client_request(int client_fd, ht_t *user_table);
void process_login_request(int client_fd, ht_t *user_table, 
				            const char *username, const char *password);
bool read_client_data(int client_fd, request_type_t *request_type, 
			char *username, size_t username_size, char *password, 
			size_t password_size);
bool create_user(ht_t *user_table, const char *username, const char *password);
void send_login_response(int client_fd, bool login_success);

#endif