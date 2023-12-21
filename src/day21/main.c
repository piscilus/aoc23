/*
 * Main program for advent of code 2023 day 21.
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
// #include <unistd.h>

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 21: Step Counter\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* grid = grid2d_init(argv[1]);
    if (grid == NULL)
        exit(EXIT_FAILURE);

    printf("%zu %zu\n", grid->max_y, grid->max_x);

    size_t n = 64;
    while (n-- > 0)
    {
        for (size_t y = 0U; y < grid->max_y; y++)
        {
            for (size_t x = 0U; x <= grid->max_x; x++)
            {
                if (grid->grid[y][x] == 'S' || grid->grid[y][x] == 'O')
                {
                    if ((y > 0) && (grid->grid[y - 1][x] == '.'))
                        grid->grid[y - 1][x] = 'N';
                    if ((y < (grid->max_y - 1)) && (grid->grid[y + 1][x] == '.'))
                        grid->grid[y + 1][x] = 'N';
                    if ((x > 0) && (grid->grid[y][x - 1] == '.'))
                        grid->grid[y][x-1] = 'N';
                    if ((x < (grid->max_x - 1)) && (grid->grid[y][x + 1] == '.'))
                        grid->grid[y][x + 1] = 'N';
                }
            }
        }

        for (size_t y = 0U; y < grid->max_y; y++)
        {
            for (size_t x = 0U; x <= grid->max_x; x++)
            {
                if (grid->grid[y][x] == 'O')
                    grid->grid[y][x] = '.';
                if (grid->grid[y][x] == 'N')
                    grid->grid[y][x] = 'O';
            }
        }

        // // system("clear");
        // for (size_t y = 0U; y < grid->max_y; y++)
        // {
        //     for (size_t x = 0U; x <= grid->max_x; x++)
        //         putchar(grid->grid[y][x]);
        //     putchar('\n');
        // }
        // usleep(500 * 1000);
    }

    size_t count = 1; // + start
    for (size_t y = 0U; y < grid->max_y; y++)
        for (size_t x = 0U; x <= grid->max_x; x++)
            if ( grid->grid[y][x] == 'O')
                count++;
    printf("Part 1: %zu\n", count);

    grid2d_destroy(grid);

    return EXIT_SUCCESS;
}
