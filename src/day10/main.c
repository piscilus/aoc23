/*
 * Main program for advent of code 2023 day 10.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "grid2d.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef enum
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
} dir_t;

typedef struct
{
    size_t x;
    size_t y;
} coords_t;

grid2d_t* grid = NULL;

int
check_field(size_t y, size_t x, dir_t dir)
{
    const char next_dir[][4] =
    {
        [UP]    = { '|', '7', 'F', 'S'},
        [DOWN]  = { '|', 'J', 'L', 'S'},
        [LEFT]  = { '-', 'L', 'F', 'S'},
        [RIGHT] = { '-', 'J', '7', 'S'}
    };

    for (size_t i = 0U; i < 4U; i++)
        if ((grid->grid[y][x] == next_dir[dir][i]) && (grid->flags[y][x] == 0U))
            return 1;
    return 0;
}

int
try_move_up(coords_t* current)
{
    if (   (current->y > 0U)
        && check_field(current->y - 1U, current->x, UP))
    {
        current->y = current->y - 1U;
        return 1;
    }
    return 0;
}

int
try_move_down(coords_t* current)
{
    if (   (current->y < (grid->max_y - 1U))
        && check_field(current->y + 1U, current->x, DOWN))
    {
        current->y = current->y + 1U;
        return 1;
    }
    return 0;
}

int
try_move_left(coords_t* current)
{
    if (   (current->x > 0U)
        && check_field(current->y, current->x - 1U, LEFT))
    {
        current->x = current->x - 1U;
        return 1;
    }
    return 0;
}

int
try_move_right(coords_t* current)
{
    if (   (current->x < (grid->max_x - 1U))
        && check_field(current->y, current->x + 1U, RIGHT))
    {
        current->x = current->x + 1U;
        return 1;
    }
    return 0;
}

coords_t*
find_start(grid2d_t* g, coords_t* pos)
{
    for (size_t y = 0U; y < g->max_y; y++)
    {
        for (size_t x = 0U; x < g->max_x; x++)
        {
            if (g->grid[y][x] == 'S')
            {
                pos->x = x;
                pos->y = y;
                return pos;
            }
        }
    }
    return NULL;
}

int
move(coords_t* current)
{
    switch (grid->grid[current->y][current->x])
    {
        case '|': /* up or down */
            if (try_move_up(current))
                return 1;
            if (try_move_down(current))
                return 1;
            break;
        case '-': /* left or right */
            if (try_move_left(current))
                return 1;
            if (try_move_right(current))
                return 1;
            break;
        case 'L':
            /* up or right */
            if (try_move_up(current))
                return 1;
            if (try_move_right(current))
                return 1;
            break;
        case 'J':
            /* up or left */
            if (try_move_up(current))
                return 1;
            if (try_move_left(current))
                return 1;
            break;
        case '7':
            /* left or down */
            if (try_move_left(current))
                return 1;
            if (try_move_down(current))
                return 1;
            break;
        case 'F':
            /* down or right */
            if (try_move_down(current))
                return 1;
            if (try_move_right(current))
                return 1;
            break;
        case 'S':
            if (try_move_up(current))
                return 1;
            if (try_move_down(current))
                return 1;
            if (try_move_left(current))
                return 1;
            if (try_move_right(current))
                return 1;
            break;
        default:
            break;
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 10: Pipe Maze\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid = grid2d_init(argv[1]);
    if (grid == NULL)
        exit(EXIT_FAILURE);

    coords_t current;
    if (find_start(grid, &current) == NULL)
        exit(EXIT_FAILURE);
    coords_t start = current;

    size_t steps = 0U;
    int moved;
    do
    {
        grid->flags[current.y][current.x] = 1;
        moved = move(&current);
        if ((start.x == current.x) && (start.y == current.y))
            moved = 0; /* back at start */
        steps++;
    }
    while((moved != 0) && (steps < (grid->max_x * grid->max_y)));

    printf("Part 1: %zu\n", steps / 2U);

    grid2d_destroy(grid);

    return EXIT_SUCCESS;
}
