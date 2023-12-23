/*
 * Main program for advent of code 2023 day 11.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "grid2d.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

typedef struct
{
    size_t x;
    size_t y;
} coord_t;

static size_t
count_smaller(size_t* a, size_t n, size_t v)
{
    size_t count = 0U;
    for (size_t i = 0U; i < n; i++)
        if (v > a[i])
            count++;
    return count;
}

static size_t
calc_distances(const grid2d_t* grid, size_t* y_spaces, size_t num_y_spaces, size_t* x_spaces, size_t num_x_spaces, size_t expand)
{
    size_t num_galaxies = 0U;
    for (size_t y = 0U; y < grid->max_y; y++)
        for (size_t x = 0U; x < grid->max_x; x++)
            if (grid->grid[y][x] == '#')
                num_galaxies++;

    coord_t* galaxies = (coord_t*)malloc(num_galaxies * sizeof(coord_t));
    if (galaxies == NULL)
        exit(EXIT_FAILURE);

    size_t count = 0U;
    for (size_t y = 0U; y < grid->max_y; y++)
    {
        for (size_t x = 0U; x < grid->max_x; x++)
        {
            if (grid->grid[y][x] == '#')
            {
                galaxies[count].y = y + count_smaller(y_spaces, num_y_spaces, y) * (expand - 1U);
                galaxies[count].x = x + count_smaller(x_spaces, num_x_spaces, x) * (expand - 1U);
                count++;
            }
        }
    }

    size_t sum = 0;
    for (size_t i = 0U; i < num_galaxies; i++)
    {
        for (size_t j = 0U; j < num_galaxies; j++)
        {
            if (i == j)
                continue;
            sum += DIFF(galaxies[i].x, galaxies[j].x) + DIFF(galaxies[i].y, galaxies[j].y);
        }
    }

    free(galaxies);

    return sum / 2U;
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 11: Cosmic Expansion\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* grid = grid2d_init(argv[1]);
    if (grid == NULL)
        exit(EXIT_FAILURE);

    size_t* y_spaces = (size_t*)malloc(grid->max_y * sizeof(size_t));
    if (y_spaces == NULL)
        exit(EXIT_FAILURE);
    size_t num_y_spaces = 0U;
    for (size_t y = 0U; y < grid->max_y; y++)
    {
        int empty = 1;
        for (size_t x = 0U; x < grid->max_x; x++)
        {
            if (grid->grid[y][x] != '.')
            {
                empty = 0;
                break;
            }
        }
        if (empty)
        {
            y_spaces[num_y_spaces] = y;
            num_y_spaces++;
        }
    }

    size_t* x_spaces = (size_t*)malloc(grid->max_x * sizeof(size_t));
    if (x_spaces == NULL)
        exit(EXIT_FAILURE);
    size_t num_x_spaces = 0U;
    for (size_t x = 0U; x < grid->max_x; x++)
    {
        int empty = 1;
        for (size_t y = 0U; y < grid->max_y; y++)
        {
            if (grid->grid[y][x] != '.')
            {
                empty = 0;
                break;
            }
        }
        if (empty)
        {
            x_spaces[num_x_spaces] = x;
            num_x_spaces++;
        }
    }

    size_t distance;

    distance = calc_distances(grid, y_spaces, num_y_spaces, x_spaces, num_x_spaces, 2);
    printf("Part 1: %zu\n", distance);

    distance = calc_distances(grid, y_spaces, num_y_spaces, x_spaces, num_x_spaces, 1000000);
    printf("Part 2: %zu\n", distance);

    free(y_spaces);
    free(x_spaces);

    grid2d_destroy(grid);

    return EXIT_SUCCESS;
}
