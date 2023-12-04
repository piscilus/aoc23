/*
 * Get data from a file, preprocess and provide access to Null terminated lines.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef DATA_H_
#define DATA_H_

#include <stdlib.h>

typedef struct data data_t;

data_t*
data_init(char* file_path);

size_t
data_num_lines(data_t* d);

void
data_reset(data_t* d);

char*
data_set_line(data_t* d, size_t n);

char*
data_next_line(data_t* d);

void
data_destroy(data_t* d);

#endif /* DATA_H_ */
