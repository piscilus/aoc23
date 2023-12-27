/*
 * Main program for advent of code 2023 day 13.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH (200)

static int
compare(uint32_t data[], size_t data_len)
{
    int ref_pos = 0;
    size_t n = SIZE_MAX;
    size_t start = 1U;

    while ((start < data_len) && (ref_pos == 0))
    {
        ref_pos = 0;
        for (size_t x = start; x < data_len; x++)
        {
            if (data[x - 1U] == data[x])
            {
                n = x;
                break;
            }
        }

        if (n < SIZE_MAX)
        {
            ref_pos = n;
            for (size_t i = 0U; i < (n - 1U); i++)
            {
                if (((n + i + 1U) >= data_len))
                    break;
                if ((data[n - i - 2U] != data[n + i + 1U]))
                    ref_pos = 0;
            }
        }
        start++;
    }

    return ref_pos;
}

static int
find_reflections(char* grid[], size_t height)
{
    assert(height > 1U);
    size_t width = strlen(grid[0]);
    assert(width > 0U);
    width--;
    uint32_t* rows = (uint32_t*)calloc(height, sizeof(uint32_t));
    if (rows == NULL)
        return 0;
    uint32_t* cols = (uint32_t*)calloc(width, sizeof(uint32_t));
    if (cols == NULL)
    {
        free(rows);
        return 0;
    }

    for (size_t y = 0U; y < height; y++)
    {
        uint32_t hash = 5381U;
        for (size_t x = 0U; x < width; x++)
            hash = ((hash << 5) + hash) + grid[y][x];
        rows[y] = hash;
    }

    for (size_t x = 0U; x < width; x++)
    {
        uint32_t hash = 5381U;
        for (size_t y = 0U; y < height; y++)
            hash = ((hash << 5) + hash) + grid[y][x];
        cols[x] = hash;
    }

    int result;
    result = compare(cols, width);
    result = result + compare(rows, height) * 100;

    free(rows);
    free(cols);

    return result;
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 13: Point of Incidence\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char* buffer = (char*)malloc(MAX_LINE_LENGTH);
    if (buffer == NULL)
        exit(EXIT_FAILURE);

    int part1 = 0;
    char** grid = NULL;
    size_t height = 0U;
    while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL)
    {
        if (strcmp(buffer, "\n") == 0)
        {
            if (height > 0U)
            {
                part1 += find_reflections(grid, height);
                for (size_t i = 0U; i < height; i++)
                    free(grid[i]);
                free(grid);
                height = 0U;
            }
        }
        else
        {
            if (height == 0U)
            {
                grid = (char**)malloc(sizeof(char*));
                if (grid == NULL)
                    exit(EXIT_FAILURE);
            }
            else
            {
                grid = (char**)realloc(grid, (height + 1U) * sizeof(char*));
                if (grid == NULL)
                    exit(EXIT_FAILURE);
            }
            grid[height] = strdup(buffer);
            if (grid[height] == NULL)
                exit(EXIT_FAILURE);
            height++;
        }
    }
    if (height > 0U)
    {
        part1 += find_reflections(grid, height);
        for (size_t i = 0U; i < height; i++)
            free(grid[i]);
        free(grid);
    }

    printf("Part 1: %d\n", part1);

    free(buffer);
    fclose(fp);

    return EXIT_SUCCESS;
}



