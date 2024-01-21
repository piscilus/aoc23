/*
 * Manage a 2D rectangular grid read from a file.
 *
 * Each row must end with a newline character (\n).
 * First line determines number of rows.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef GRID2D_H_
#define GRID2D_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    FILE* fp;
    size_t file_size;  /* w/out null terminator */
    char* file_buffer; /* complete file content null terminated */
    char** grid;
    size_t** flags;
    size_t max_x;
    size_t max_y;
} grid2d_t;

grid2d_t*
grid2d_init(char* file_path);

void
grid2d_print(grid2d_t* g);

void
grid2d_destroy(grid2d_t* g);

#endif /* GRID2D_H_ */
