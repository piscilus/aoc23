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
determine_file_size(grid2d_t* g)
{
    assert(g != NULL);
    assert(g->fp != NULL);

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

    if (fread(g->file_buffer, 1, g->file_size, g->fp) != g->file_size)
    {
        free(g->file_buffer);
        return NULL;
    }

    g->file_buffer[g->file_size] = '\0';

    return g;
}

static grid2d_t*
prepare_grid(grid2d_t* g)
{
    assert(g != NULL);

    /* determine max y by counting EOL */
    for (size_t i = 0U; i < g->file_size; i++)
        if (g->file_buffer[i] == '\n')
            g->max_y++;

    g->grid = (char**)malloc(g->max_y * sizeof(char*));
    if (g->grid == NULL)
        return NULL;

    char *p = g->file_buffer;
    /* determine max x by counting until EOL in the first line */
    while (*(p++) != '\n')
        g->max_x++;

    p = g->file_buffer;
    for (size_t i = 0U; i < g->max_y; i++)
    {
        g->grid[i] = p;
        size_t line_length = strcspn(p, "\n");
        if (line_length != g->max_x)
        {
            free(g->grid);
            return NULL;
        }
        g->grid[i][line_length] = '\0';
        p += line_length + 1U;
    }

    g->flags = (size_t**)calloc(g->max_y, sizeof(size_t*));
    if (g->grid == NULL)
    {
        free(g->grid);
        return NULL;
    }

    for (size_t y = 0; y < g->max_y; y++)
    {
        g->flags[y] = (size_t*)calloc(g->max_x, sizeof(size_t));
        if (g->flags[y] == NULL) {
            free(g->grid);
            free(g->flags);
            return NULL;
        }
    }

    return g;
}


grid2d_t*
grid2d_init(char* file_path)
{
    if (file_path == NULL)
        return NULL;

    grid2d_t* g = (grid2d_t*)malloc(sizeof(grid2d_t));
    if (g == NULL)
    {
        perror("Memory allocation error");
        return NULL;
    }

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

    if (prepare_grid(g) == NULL)
    {
        perror("Could not prepare grid");
        fclose(g->fp);
        free(g);
        return NULL;
    }

    fclose(g->fp);

    return g;
}

void
grid2d_print(grid2d_t* g)
{
    if ((g == NULL) || (g->grid == NULL))
        return;

    printf("x=%zu, y=%zu:\n", g->max_x, g->max_y);
    for (size_t i = 0U; i < g->max_y; i++)
        printf("%s\n", g->grid[i]);
}

void
grid2d_destroy(grid2d_t* g)
{
    free(g->file_buffer);
    for (size_t y = 0U; y < g->max_y; y++)
        free(g->flags[y]);
    free(g->flags);
    free(g->grid);
    free(g);

    return;
}
