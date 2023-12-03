/*
 * Manage a 2D rectangular grid.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "grid2d.h"

#include <assert.h>
#include <string.h>

static grid2d_t*
determine_file_size(grid2d_t* g);

static grid2d_t*
read_file_content(grid2d_t* g);

static grid2d_t*
store_grid(grid2d_t* g);

grid2d_t*
grid2d_init(char* file_path)
{
    if (file_path == NULL)
        return NULL;

    grid2d_t* g = (grid2d_t*)malloc(sizeof(grid2d_t));
    if (g == NULL)
        return NULL;

    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        perror("Could not open file");
        free(g);
        return NULL;
    }

    g->fp = fp;
    if (determine_file_size(g) == NULL)
    {
        perror("Could not determine file size");
        fclose(g->fp);
        free(g);
        return NULL;
    }

    if (read_file_content(g) == NULL)
    {
        perror("Could not read file to buffer");
        fclose(g->fp);
        free(g);
        return NULL;
    }

    if (store_grid(g) == NULL)
    {
        perror("Could not parse lines");
        fclose(g->fp);
        free(g->file_buffer);
        free(g);
        return NULL;
    }

    fclose(g->fp);

    return g;
}

void
grid2d_print(grid2d_t* g)
{
    for (size_t i = 0U; i < g->max_y; i++)
    {
        printf("%s\n", g->grid[i]);
    }
}

void
grid2d_destroy(grid2d_t* d)
{
    free(d->file_buffer);
    free(d->grid);
    free(d);

    return;
}

static grid2d_t*
determine_file_size(grid2d_t* g)
{
    if (fseek(g->fp, 0, SEEK_END) != 0)
        return NULL;

    long file_size;
    if ((file_size = ftell(g->fp)) < 0)
        return NULL;

    if (fseek(g->fp, 0, SEEK_SET) != 0)
        return NULL;

    g->file_size = (size_t)file_size;

    return g;
}

static grid2d_t*
read_file_content(grid2d_t* g)
{
    assert(g != NULL);
    assert(g->fp != NULL);

    if (g->file_size == 0U)
        return NULL;

    g->file_buffer = (char*)malloc(g->file_size + 1U);
    if (g->file_buffer == NULL)
        return NULL;

    fread(g->file_buffer, 1, g->file_size, g->fp);
    g->file_buffer[g->file_size] = '\0';

    return g;
}

static grid2d_t*
store_grid(grid2d_t* g)
{
    assert(g != NULL);

    for (size_t i = 0U; i < g->file_size; i++)
        if (g->file_buffer[i] == '\n') /* \r not supported */
            g->max_y++;

    g->grid = (char**)malloc(g->max_y * sizeof(char*));

    if (g->grid == NULL)
    {
        perror("Memory allocation error");
        return NULL;
    }

    char *currentLine = g->file_buffer;

    while (*(currentLine++) != '\n')
    {
        g->max_x++;
    }

    currentLine = g->file_buffer;
    for (size_t i = 0U; i < g->max_y; i++)
    {
        g->grid[i] = currentLine;
        currentLine = strchr(currentLine, '\n');
        if (currentLine != NULL)
        {
            *currentLine = '\0';
            currentLine++;
        }
    }

    return g;
}

