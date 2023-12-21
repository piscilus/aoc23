/**
 * \file queue.c
 *
 * \copyright (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "queue.h"

#include <assert.h>
#include <string.h>

typedef struct node
{
    void* item;
    struct node* next;
} node_t;

struct queue_list
{
    size_t count;
    size_t item_size;
    node_t* head;
    node_t* tail;
};

queue_t*
queue_init(size_t item_size)
{
    if (item_size == 0U)
        return NULL;

    queue_t* q = (queue_t*)malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;

    q->count = 0U;
    q->item_size = item_size;
    q->head = NULL;
    q->tail = NULL;

    return q;
}

bool
queue_enqueue(queue_t* q, const void* item)
{
    assert(q != NULL);
    assert(item != NULL);

    node_t* new = (node_t*)malloc(sizeof(node_t));
    if (new == NULL)
        return false;

    new->item = malloc(q->item_size);
    if (new->item == NULL)
    {
        free(new);
        return false;
    }

    new->next = NULL;

    (void)memcpy(new->item, item, q->item_size);

    if (q->tail == NULL)
    {
        q->head = new;
        q->tail = new;
    }
    else
    {
        q->tail->next = new;
        q->tail = new;
    }

    q->count++;

    return true;
}

bool
queue_dequeue(queue_t* q, void* item)
{
    assert(q != NULL);
    assert(item != NULL);

    if (q->head == NULL)
        return false;

    node_t* tmp = q->head;
    (void)memcpy(item, tmp->item, q->item_size);
    q->head = tmp->next;
    if (q->head == NULL)
        q->tail = NULL;

    assert(q->count > 0U);
    q->count--;
    free(tmp->item);
    free(tmp);

    return true;
}

bool
queue_peek(const queue_t* q, void* item)
{
    assert(q != NULL);
    assert(item != NULL);

    if (q->head == NULL)
        return false;

    node_t* tmp = q->head;
    (void)memcpy(item, tmp->item, q->item_size);
    return true;
}

size_t
queue_count(const queue_t* q)
{
    return q->count;
}

void
queue_destroy(queue_t* q)
{
    node_t* tmp;

    if (q == NULL)
        return;

    while (q->head != NULL)
    {
        tmp = q->head;
        q->head = tmp->next;
        free(tmp->item);
        free(tmp);
    }

    q->count = 0U;
    q->item_size = 0U;
    q->head = NULL;
    q->tail = NULL;
}
