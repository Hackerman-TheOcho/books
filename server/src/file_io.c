#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_io.h"

void load_users(ht_t *user_table, const char *filename) 
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		return;
	}

	char line[1024];
	while (fgets(line, sizeof(line), file)) {
		// Remove newline
		line[strcspn(line, "\n")] = 0;

		// Split line into username and password
		char *username = strtok(line, ",");
		char *password = strtok(NULL, ",");

		if (username && password) {
			// Insert into hash table
			ht_insert(user_table, username, strdup(password));
		}
	}

	fclose(file);
}

static void save_entry_callback(const char *key, void *obj, void *data)
{
	FILE *file = (FILE *)data;

	// assumes obj is a string (password)
    	fprintf(file, "%s,%s\n", key, (char *)obj);
}

void save_users(ht_t *user_table, const char *filename) 
{
	FILE *file = fopen(filename, "w");
	if (!file) {
		perror("Error opening file");
		return;
	}

	ht_iter(user_table, save_entry_callback, file);

	fclose(file);
}
