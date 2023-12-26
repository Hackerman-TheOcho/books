#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "ht.h"

typedef struct entry {
	char *key;
	void *obj;
	struct entry *next;
} entry;

struct _ht_t {
	uint32_t size;
	hashfunction *hash;
	cleanup_t *cleanup;
	entry **elements;
	pthread_mutex_t mutex;
};


static size_t ht_index(ht_t *ht, const char *key) {
	size_t result = (ht->hash(key, strlen(key)) % ht->size);
	return result;
}

// pass in NULL cf for default free behavior
ht_t *ht_create(uint32_t size, hashfunction *hf, cleanup_t *cf) 
{
	ht_t *ht = malloc(sizeof(*ht));
	ht->size = size;
	ht->hash = hf;

	if (cf) {
		ht->cleanup = cf;
	} else {
		ht->cleanup = free;
	}

	ht->elements = calloc(sizeof(entry*), ht->size);
	pthread_mutex_init(&ht->mutex, NULL);
	return ht;
}

void ht_destroy(ht_t *ht)
{
	// lock before destroying
	pthread_mutex_lock(&ht->mutex);

	for (uint32_t i = 0; i < ht->size; i++) {
		while (ht->elements[i]) {
			entry *temp = ht->elements[i];
			ht->elements[i] = ht->elements[i]->next;
			free(temp->key);
			ht->cleanup(temp->obj);
			free(temp);
		}
	}
	free(ht->elements);

	// unlock and destroy mutex
	pthread_mutex_unlock(&ht->mutex);
	pthread_mutex_destroy(&ht->mutex);

	free(ht);
}

void ht_printf(ht_t *ht)
{
	printf("Start Table\n");
	for (uint32_t i = 0; i < ht->size; i++) {
		if (ht->elements[i] == NULL) {
			printf("\t%i\t---\n", i);
		} else {
			printf("\t%i\t\n", i);
			entry *temp = ht->elements[i];
			while (temp != NULL) {
				printf("\"%s\"(%p) - ", temp->key, temp->obj);
				temp = temp->next;
			}
			printf("\n");
		}
	}
	printf("End Table\n");
}

bool ht_insert(ht_t *ht, const char *key, void *obj)
{
	if (key == NULL || obj == NULL || ht == NULL) {
		return false;
	}

	// lock mutex
	pthread_mutex_lock(&ht->mutex);

	size_t index = ht_index(ht, key);

	if (ht_search(ht, key) != NULL) {
		return false;
	}

	// create new entry
	entry *e = malloc(sizeof(*e));
	e->obj =obj;
	e->key = malloc(strlen(key) + 1);
	strcpy(e->key, key);
	// e->key = strdup(key);

	// insert entry
	e->next = ht->elements[index];
	ht->elements[index] = e;

	// unlock mutex
	pthread_mutex_unlock(&ht->mutex);

	return true;
}

void *ht_search(ht_t *ht, const char *key)
{
	if (key == NULL || ht == NULL) {
		return false;
	}

	// lock mutex
	pthread_mutex_lock(&ht->mutex);

	size_t index = ht_index(ht, key);

	entry *temp = ht->elements[index];
	while (temp != NULL && strcmp(temp->key, key) != 0) {
		temp = temp->next;
	}

	if (temp == NULL) {
		return NULL;
	}

	// unlock mutex
	pthread_mutex_unlock(&ht->mutex);

	return temp->obj;
}

void *ht_delete(ht_t *ht, const char *key)
{
	if (key == NULL || ht == NULL) {
		return false;
	}

	// lock mutex
	pthread_mutex_lock(&ht->mutex);

	size_t index = ht_index(ht, key);

	entry *temp = ht->elements[index];
	entry *prev = NULL;
	while (temp != NULL && strcmp(temp->key, key) != 0) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		return NULL;
	}

	if (prev == NULL) {
		// delete head of list
		ht->elements[index] = temp->next;
	} else {
		// delete from somewhere other than head
		prev->next = temp->next;
	}

	void *result = temp->obj;
	
	// unlock mutex
	pthread_mutex_unlock(&ht->mutex);

	free(temp);
	return result;
}

void ht_iter(ht_t *ht, ht_iter_callback callback, void *data)
{
	if (!ht || !callback) {
		return;
	}

	for (uint32_t i = 0; i < ht->size; ++i) {
		for (entry *e = ht->elements[i]; e != NULL; e = e->next) {
			callback(e->key, e->obj, data);
		}
	}
}