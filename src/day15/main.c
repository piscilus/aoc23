/*
 * Main program for advent of code 2023 day 15.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HT_SIZE (256U)

typedef struct node
{
    char* key;
    char* value;
    struct node* next;
} node_t;

typedef struct
{
    node_t* table[HT_SIZE];
} hashtable_t;

static size_t
calc_hash(char* p, size_t s)
{
    size_t hash = 0U;
    while ((s-- > 0U) && (*p != '\0'))
        hash = ((hash + *p++) * 17U) % 256U;
    return hash;
}

static size_t
get_key_len(char* p)
{
    size_t key_len = 0U;
    while ((*p != '\0') && (*p != '-') && (*p != '='))
    {
        key_len++;
        p++;
    }
    return key_len;
}

static void
ht_add_or_replace(hashtable_t* ht, char* data, size_t key_len)
{
    size_t i = calc_hash(data, key_len);

    node_t* new = (node_t*)malloc(sizeof(node_t));
    if (new == NULL)
    {
        fprintf(stderr, "Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    new->key = strndup(data, key_len);
    new->value = strdup(data);
    new->next = NULL;

    if (ht->table[i] == NULL)
    {
        ht->table[i] = new;
    }
    else
    {
        node_t* current = ht->table[i];
        node_t* prev = NULL;
        while ((current != NULL) && (strncmp(current->value, data, key_len) != 0))
        {
            prev = current;
            current = current->next;
        }
        if (current != NULL)
        {
            free(current->key);
            free(current->value);
            current->key = strndup(data, key_len);
            current->value = strdup(data);
        }
        else
        {
            if (prev != NULL)
                prev->next = new;
            else
                ht->table[i] = new;
        }
    }
}

static void
ht_try_remove(hashtable_t* ht, char* key, size_t key_len)
{
    size_t i = calc_hash(key, key_len);

    node_t* current = ht->table[i];
    node_t* prev = NULL;

    while ((current != NULL) && (strncmp(current->key, key, key_len) != 0))
    {
        prev = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (prev != NULL)
            prev->next = current->next;
        else
            ht->table[i] = current->next;
        free(current->key);
        free(current->value);
        free(current);
    }
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 15: Lens Library\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    size_t part1 = 0U;
    char* l = data_set_line(d, 0U);
    char* t = strtok(l, ",");
    while (t != NULL)
    {
        part1 += calc_hash(t, strlen(t));
        t = strtok(NULL, ",");
    }

    printf("Part 1: %zu\n", part1);
    data_destroy(d);

    d = data_init(argv[1]);

    hashtable_t ht;
    for (size_t i = 0U; i < HT_SIZE; i++)
        ht.table[i] = NULL;

    l = data_set_line(d, 0U);
    t = strtok(l, ",");
    while (t != NULL)
    {
        size_t key_len = get_key_len(t);
        if (t[key_len] == '=')
            ht_add_or_replace(&ht, t, key_len);
        else if (t[key_len] == '-')
            ht_try_remove(&ht, t, key_len);
        else
            assert(0);
        t = strtok(NULL, ",");
    }

    size_t part2 = 0U;
    for (size_t i = 0U; i < HT_SIZE; i++)
    {
        node_t* current = ht.table[i];
        size_t n = 1U;
        while (current != NULL)
        {
            size_t j = get_key_len(current->value);
            assert(current->value[j + 1U] >= '0');
            part2 += (i + 1U) * n * (current->value[j + 1U] - '0');
            node_t* temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
            n++;
        }
    }

    printf("Part 2: %zu\n", part2);
    data_destroy(d);

    return EXIT_SUCCESS;
}
