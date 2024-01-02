/*
 * One-dimensional dynamic array.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef DA1D_H_
#define DA1D_H_

#include <stdlib.h>

typedef struct da1d da1d_t;

da1d_t*
da1d_init(size_t num_elements, size_t element_size);

size_t
da1d_get_num(const da1d_t* a);

void*
da1d_get(const da1d_t* a, size_t element);

void*
da1d_set(da1d_t* a, size_t element, const void* data);

void*
da1d_resize(da1d_t* a, size_t num_elements);

void
da1d_destroy(da1d_t* a);

#endif /* DA1D_H_ */
