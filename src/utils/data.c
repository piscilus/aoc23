/*
 * Get data from a file, preprocess and provide access to Null terminated lines.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

struct data
{
    FILE* fp;
    size_t file_size; /* w/out null terminator */
    char* file_buffer; /* complete file content null terminated */
    char** lines;  /* list of pointers to line beginnings */
    size_t num_lines;
    size_t current_line;
};

static data_t*
determine_file_size(data_t* d)
{
    assert(d != NULL);
    assert(d->fp != NULL);

    if (fseek(d->fp, 0, SEEK_END) != 0)
        return NULL;

    long file_size;
    if ((file_size = ftell(d->fp)) < 0)
        return NULL;

    if (fseek(d->fp, 0, SEEK_SET) != 0)
        return NULL;

    d->file_size = (size_t)file_size;

    return d;
}

static data_t*
read_file_content(data_t* d)
{
    assert(d != NULL);
    assert(d->fp != NULL);

    if (d->file_size == 0U)
        return NULL;

    d->file_buffer = (char*)malloc(d->file_size + 1U);
    if (d->file_buffer == NULL)
        return NULL;

    if (fread(d->file_buffer, 1, d->file_size, d->fp) != d->file_size)
    {
        free(d->file_buffer);
        return NULL;
    }

    d->file_buffer[d->file_size] = '\0';

    return d;
}

static data_t*
detect_lines(data_t* d)
{
    assert(d != NULL);

    for (size_t i = 0U; i < d->file_size; i++)
        if (d->file_buffer[i] == '\n') /* \r not supported */
            d->num_lines++;

    d->lines = (char**)malloc(d->num_lines * sizeof(char*));

    if (d->lines == NULL)
        return NULL;

    char *currentLine = d->file_buffer;
    for (size_t i = 0U; i < d->num_lines; i++)
    {
        d->lines[i] = currentLine;
        currentLine = strchr(currentLine, '\n');
        if (currentLine != NULL)
        {
            *currentLine = '\0';
            currentLine++;
        }
    }

    return d;
}

data_t*
data_init(char* file_path)
{
    if (file_path == NULL)
        return NULL;

    data_t* d = (data_t*)malloc(sizeof(data_t));
    if (d == NULL)
        return NULL;

    FILE* fp = fopen(file_path, "r");
    if (!fp)
    {
        perror("Could not open file");
        free(d);
        return NULL;
    }

    d->fp = fp;
    if (determine_file_size(d) == NULL)
    {
        perror("Could not determine file size");
        fclose(d->fp);
        free(d);
        return NULL;
    }

    if (read_file_content(d) == NULL)
    {
        perror("Could not read file to buffer");
        fclose(d->fp);
        free(d);
        return NULL;
    }

    if (detect_lines(d) == NULL)
    {
        perror("Could not parse lines");
        fclose(d->fp);
        free(d->file_buffer);
        free(d);
        return NULL;
    }
    d->current_line = 0U;

    fclose(d->fp);

    return d;
}

size_t
data_num_lines(data_t* d)
{
    if (d != NULL)
        return d->num_lines;
    else
        return 0U;
}

void
data_reset(data_t* d)
{
    if (d != NULL)
        d->current_line = 0;
}

char*
data_set_line(data_t* d, size_t n)
{
    if ((d == NULL) || (n > (d->num_lines - 1)))
        return NULL;

    d->current_line = n;

    return d->lines[d->current_line++];
}

char*
data_next_line(data_t* d)
{
    if ((d == NULL) || (d->lines == NULL))
        return NULL;
    if (d->current_line < d->num_lines)
        return d->lines[d->current_line++];
    else
        return NULL;
}

void
data_destroy(data_t* d)
{
    free(d->file_buffer);
    free(d->lines);
    free(d);

    return;
}
