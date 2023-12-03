/*
 * Main program for advent of code 2023 day 3.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "grid2d.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This one needs refactoring :-) */

typedef struct
{
    size_t x;
    size_t y;
    int count; /* number of adjacent part numbers */
    int ratio;
} gear_t;

typedef struct
{
    gear_t** gears; /* list of all gears on the grid */
    size_t num_gears;
    size_t max_gears;
} gears_t;

grid2d_t* grid = NULL;
gears_t gears = {0};

void
gears_init(void)
{
    for (size_t i = 0U; i < grid->file_size; i++)
        if (grid->file_buffer[i] == '*')
            gears.max_gears++;

    gears.gears = malloc(sizeof(gear_t*) * gears.max_gears);
    assert(gears.gears != NULL);
    for (size_t i = 0U; i < gears.max_gears; i++)
    {
        gears.gears[i] = malloc(sizeof(gear_t));
        assert(gears.gears[i] != NULL);
        gears.gears[i]->count = 0U;
        gears.gears[i]->x = SIZE_MAX;
        gears.gears[i]->y = SIZE_MAX;
    }
    gears.num_gears = 0U;
}

void
add_gear(size_t y, size_t x, int val)
{
    if (grid->grid[y][x] != '*')
        return;
    for (size_t i = 0U; i < gears.num_gears; i++)
    {
        if ((y == gears.gears[i]->y) && (x == gears.gears[i]->x))
        {
            gears.gears[i]->count++;
            gears.gears[i]->ratio *= val;
            return;
        }
    }
    assert (gears.num_gears < gears.max_gears);
    gears.gears[gears.num_gears]->x = x;
    gears.gears[gears.num_gears]->y = y;
    gears.gears[gears.num_gears]->count = 1;
    gears.gears[gears.num_gears]->ratio = val;
    gears.num_gears++;
}

bool
check_neighbor(size_t y, size_t x_first, size_t x_last, int val)
{
    bool res = false;

    if ((x_first > 0U) && (grid->grid[y][x_first - 1U] != '.'))
    {
        add_gear(y, x_first - 1U, val);
        res = true;
    }

    if ((x_last < (grid->max_x - 1U)) && (grid->grid[y][x_last + 1U] != '.'))
    {
        add_gear(y, x_last + 1U, val);
        res = true;
    }

    if (x_last < (grid->max_x - 1U))
        x_last = x_last + 1U;
    if (x_first > 0U)
        x_first = x_first - 1U;

    if (y < (grid->max_y - 1U))
    {
        for (size_t xx = x_first; xx <= x_last; xx++)
        {
            if (grid->grid[y + 1U][xx] != '.')
            {
                add_gear(y + 1U, xx, val);
                res = true;
            }
        }
    }

    if (y > 0U)
    {
        for (size_t xx = x_first; xx <= x_last; xx++)
        {
            if (grid->grid[y - 1U][xx] != '.')
            {
                add_gear(y - 1U, xx, val);
                res = true;
            }
        }
    }

    return res;
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 3: Gear Ratios\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid = grid2d_init(argv[1]);
    if (grid == NULL)
        exit(EXIT_FAILURE);

    gears_init();

    int part1 = 0;
    int digits[10] = {0};
    size_t m = 0U;
    for (size_t y = 0U; y < grid->max_y; y++)
    {
        for (size_t x = 0U; x <= grid->max_x; x++)
        {
            if (isdigit(grid->grid[y][x]))
            {
                digits[m++] = grid->grid[y][x] - 0x30;
            }
            else
            {
                if (m != 0U)
                {
                    int k = 0;
                    for (size_t j = 0U; j < m; j++)
                    {
                        int mul = 1;
                        for (size_t o = 0U; o < m - j - 1U; o++)
                            mul *= 10;
                        k += digits[j] * mul;
                    }
                    if (check_neighbor(y, x - m, x - 1U, k))
                        part1 += k;
                    m = 0U;
                }
            }
        }
    }

    printf("Part1: Sum of all part numbers = %d\n", part1);

    int part2 = 0;
    for (size_t i = 0U; i < gears.num_gears; i++)
        if (gears.gears[i]->count == 2U)
            part2 += gears.gears[i]->ratio;

    printf("Part2: Sum of all gear ratios = %d\n", part2);

    grid2d_destroy(grid);
    for (size_t i = 0U; i < gears.max_gears; i++)
        free(gears.gears[i]);
    free(gears.gears);

    return EXIT_SUCCESS;
}
