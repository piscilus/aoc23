/*
 * Main program for advent of code 2023 day 14.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "grid2d.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 14: Parabolic Reflector Dish\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* grid = grid2d_init(argv[1]);
    if (grid == NULL)
        exit(EXIT_FAILURE);

    bool moved = true;
    while (moved)
    {
        moved = false;
        for (size_t y = 1U; y < grid->max_y; y++)
        {
            for (size_t x = 0U; x < grid->max_x; x++)
            {
                if ((grid->grid[y][x] == 'O') && (grid->grid[y - 1U][x] == '.'))
                {
                    grid->grid[y][x] = '.';
                    grid->grid[y - 1U][x] = 'O';
                    moved = true;;
                }
            }
        }
    }

    size_t part1 = 0U;
    size_t p = grid->max_y;
    for (size_t y = 0U; y < grid->max_y; y++)
    {
        for (size_t x = 0U; x < grid->max_x; x++)
        {
            if (grid->grid[y][x] == 'O')
            {
                part1 += p;
            }
        }
        p--;
    }

    printf("Part1: Total load = %zu\n", part1);

    grid2d_destroy(grid);

    return EXIT_SUCCESS;
}
