/*
 * Simple hash table implementation.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "hashtable.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct entry
{
    char* key;
    void* data;
    size_t data_size;
    struct entry* next;
} entry_t;

struct hashtable
{
    entry_t* data;
    size_t size;
    size_t used;
};

static bool
is_prime(size_t n)
{
    if (n < 2U)
        return false;

    if (n == 2U)
        return true;

    for (size_t i = 2U; (i * i) <= n; i++)
        if ((n % i) == 0U)
            return false;

    return true;
}

static size_t
next_prime(size_t n)
{
    if ((n % 2U) == 0U)
        n++;

    while (!is_prime(n))
        n += 2U;

    return n;
}

static size_t
calc_hash(const char* key) /* djb2 */
{
    size_t hash = 5381U;
    int c;

    while ((c = *key++))
        hash = ((hash << 5U) + hash) + (size_t)c;

    return hash;
}

static const char*
write(entry_t* entry, size_t size, const char* key, const void* new_data,
      size_t data_size, size_t* used)
{
    size_t hash = calc_hash(key);
    size_t i = hash & (size - 1U);

    if (entry[i].key == NULL)
    {
        char* dup_key = strdup(key);
        if (dup_key == NULL)
            return NULL;

        entry[i].key = dup_key;
        entry[i].data = malloc(data_size);
        if (entry[i].data == NULL)
        {
            free(dup_key);
            return NULL;
        }

        (void)memcpy(entry[i].data, new_data, data_size);

        if (used != NULL)
            (*used)++;

        return dup_key;
    }

    entry_t* current = &(entry[i]);
    while (current->next != NULL)
    {
        if (strcmp(key, current->key) == 0)
            return NULL;
        current = current->next;
    }

    char* dup_key = strdup(key);
    if (dup_key == NULL)
        return NULL;

    entry_t* new_entry = malloc(sizeof(entry_t));
    if (new_entry == NULL)
    {
        free(dup_key);
        return NULL;
    }

    new_entry->key = dup_key;
    new_entry->data = malloc(data_size);
    if (new_entry->data == NULL)
    {
        free(dup_key);
        free(new_entry);
        return NULL;
    }
    (void)memcpy(new_entry->data, new_data, data_size);

    current->next = new_entry;

    if (used != NULL)
        (*used)++;

    return dup_key;
}

hashtable_t*
ht_init(size_t size)
{
    if (size < 2U)
        return NULL;

    hashtable_t* ht = malloc(sizeof(hashtable_t));
    if (ht == NULL)
        return NULL;

    ht->size = next_prime(size);
    ht->used = 0U;

    ht->data = calloc(ht->size, sizeof(entry_t));
    if (ht->data == NULL)
    {
        free(ht);
        return NULL;
    }

    return ht;
}

const char*
ht_insert(hashtable_t* ht, const char* key, const void* data, size_t data_size)
{
    assert(ht != NULL);
    assert(key != NULL);
    assert(data != NULL);

    if (ht->used >= ht->size)
        return NULL;

    return write(ht->data, ht->size, key, data, data_size, &ht->used);
}

const void*
ht_search(const hashtable_t* ht, const char* key, size_t* data_size)
{
    size_t hash = calc_hash(key);
    size_t index = hash & (ht->size - 1U);

    entry_t* current = &(ht->data[index]);

    while (current != NULL)
    {
        if ((current->key != NULL) && (strcmp(key, current->key) == 0))
        {
            if (data_size != NULL)
                *data_size = current->data_size;
            return current->data;
        }
        current = current->next;
    }

    return NULL;
}

void
ht_destroy(hashtable_t* ht)
{
    if (ht == NULL)
        return;

    for (size_t i = 0U; i < ht->size; i++)
    {
        entry_t* current = &(ht->data[i]);
        if (current->key != NULL)
        {
            free(current->key);
            free(current->data);
            current = current->next;
            while (current != NULL)
            {
                entry_t* next = current->next;
                free(current->key);
                free(current->data);
                free(current);
                current = next;
            }
        }
    }

    free(ht->data);
    free(ht);
}
