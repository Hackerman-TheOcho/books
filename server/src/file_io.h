#ifndef _FILE_IO_H
#define _FILE_IO_H

#include "ht.h"

void load_users(ht_t *user_table, const char *filename);
void save_users(ht_t *user_table, const char *filename);

#endif