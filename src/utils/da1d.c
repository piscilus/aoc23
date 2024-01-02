/*
 * One-dimensional da1dmic array.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "da1d.h"

#include <assert.h>
#include <string.h>

typedef struct da1d
{
    size_t num_elements;
    size_t element_size;
    void* data;
} da1d_t;

da1d_t*
da1d_init(size_t num_elements, size_t element_size)
{
    if ((num_elements == 0U) || (element_size == 0U))
        return NULL;

    da1d_t* a = (da1d_t*)malloc(sizeof(da1d_t));
    if (a == NULL)
        return NULL;

    a->data = calloc(num_elements, element_size);
    if (a->data == NULL)
    {
        free(a);
        return NULL;
    }
    a->element_size = element_size;
    a->num_elements = num_elements;

    return a;
}

size_t
da1d_get_num(const da1d_t* a)
{
    assert(a != NULL);

    return a->num_elements;
}

void*
da1d_get(const da1d_t* a, size_t element)
{
    assert(a != NULL);

    if (element >= a->num_elements)
        return NULL;

    return (void*)((char*)a->data + (element * a->element_size));
}

void*
da1d_set(da1d_t* a, size_t element, const void* data)
{
    assert(a != NULL);
    assert(data != NULL);

    void* e = da1d_get(a, element);
    if (e == NULL)
        return NULL;

    return memcpy(e, data, a->element_size);
}

void*
da1d_resize(da1d_t* a, size_t num_elements)
{
    assert(a != NULL);

    if (num_elements == 0U)
        return NULL;

    a->data = realloc(a->data, (num_elements * a->element_size));
    if (a->data == NULL)
        return NULL;
    a->num_elements = num_elements;

    return a;
}

void
da1d_destroy(da1d_t* a)
{
    assert(a != NULL);

    free(a->data);
    free(a);
}
