/*
 * Simple hash table implementation.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdlib.h>

typedef struct hashtable hashtable_t;

hashtable_t*
ht_init(size_t size);

const char*
ht_insert(hashtable_t* ht, const char* key, const void* data, size_t data_size);

const void*
ht_search(const hashtable_t* ht, const char* key, size_t* data_size);

void
ht_destroy(hashtable_t* ht);

#endif /* HASHTABLE_H_ */
