/*
 * Main program for advent of code 2023 day 16.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "grid2d.h"
#include "queue.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    UP= 1,
    DOWN,
    LEFT,
    RIGHT
} dir_t;

typedef struct
{
    size_t x;
    size_t y;
} coord_t;

typedef struct
{
    coord_t pos;
    dir_t dir;
} beam_t;

static int
move(beam_t* beam, grid2d_t* grid)
{
    if ((beam->dir == UP) && (beam->pos.y > 0U))
    {
        beam->pos.y--;
        return 1;
    }
    if ((beam->dir == DOWN) && (beam->pos.y < (grid->max_y - 1U)))
    {
        beam->pos.y++;
       return 1;
    }
    if ((beam->dir == LEFT) && (beam->pos.x > 0U))
    {
        beam->pos.x--;
        return 1;
    }
    if ((beam->dir == RIGHT) && (beam->pos.x < (grid->max_x - 1U)))
    {
        beam->pos.x++;
        return 1;
    }

    return 0;
}

static int
redirect(beam_t* beam, grid2d_t* grid)
{
    if (   ((beam->dir == UP)   && (grid->grid[beam->pos.y][beam->pos.x] == '/'))
        || ((beam->dir == DOWN) && (grid->grid[beam->pos.y][beam->pos.x] == '\\')))
    {
        if (beam->pos.x < (grid->max_x - 1U))
        {
            beam->dir = RIGHT;
            beam->pos.x++;
            return 1;
        }
    }
    if (   ((beam->dir == UP)   && (grid->grid[beam->pos.y][beam->pos.x] == '\\'))
        || ((beam->dir == DOWN) && (grid->grid[beam->pos.y][beam->pos.x] == '/')))
    {
        if (beam->pos.x > 0U)
        {
            beam->dir = LEFT;
            beam->pos.x--;
            return 1;
        }
    }
    if (   ((beam->dir == LEFT)  && (grid->grid[beam->pos.y][beam->pos.x] == '/'))
        || ((beam->dir == RIGHT) && (grid->grid[beam->pos.y][beam->pos.x] == '\\')))
    {
        if (beam->pos.y < (grid->max_y - 1U))
        {
            beam->dir = DOWN;
            beam->pos.y++;
            return 1;
        }
    }
    if (   ((beam->dir == LEFT)  && (grid->grid[beam->pos.y][beam->pos.x] == '\\'))
        || ((beam->dir == RIGHT) && (grid->grid[beam->pos.y][beam->pos.x] == '/')))
    {
        if (beam->pos.y > 0U)
        {
            beam->dir = UP;
            beam->pos.y--;
            return 1;
        }
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 16: The Floor Will Be Lava\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    grid2d_t* grid = grid2d_init(argv[1]);
    if (grid == NULL)
        exit(EXIT_FAILURE);

    queue_t* q = queue_init(sizeof(beam_t));
    if (q== NULL)
        exit(EXIT_FAILURE);

    beam_t beam =
    {
        .dir = RIGHT,
        .pos = { .x = 0U, .y = 0U}
    };
    if (!queue_enqueue(q, &beam))
        exit(EXIT_FAILURE);

    while (queue_dequeue(q, &beam))
    {
        int alive = 1;
        while (alive)
        {
            if (grid->flags[beam.pos.y][beam.pos.x] == beam.dir)
            {
                alive = 0;
                break;
            }
            grid->flags[beam.pos.y][beam.pos.x] = beam.dir;
            switch (grid->grid[beam.pos.y][beam.pos.x])
            {
                case '.':
                    alive = move(&beam, grid);
                    break;
                case '-':
                    if ((beam.dir == LEFT) || (beam.dir == RIGHT))
                        alive = move(&beam, grid);
                    else
                    {
                        if (beam.pos.x > 0U)
                        {
                            beam.pos.x--;
                            beam.dir = LEFT;
                        }
                        else
                        {
                            alive = 0;
                        }
                        if (beam.pos.x < (grid->max_x - 1U))
                            if (!queue_enqueue(q, &(beam_t){.dir = RIGHT,.pos = { .x = beam.pos.x + 1U, .y = beam.pos.y}}))
                                exit(EXIT_FAILURE);
                    }
                    break;
                case '|':
                    if ((beam.dir == UP) || (beam.dir == DOWN))
                        alive = move(&beam, grid);
                    else
                    {
                        if (beam.pos.y > 0U)
                        {
                            beam.pos.y--;
                            beam.dir = UP;
                        }
                        else
                        {
                            alive = 0;
                        }
                        if (beam.pos.y < (grid->max_y - 1U))
                            if (!queue_enqueue(q, &(beam_t){.dir = DOWN,.pos = { .x = beam.pos.x, .y = beam.pos.y + 1U}}))
                                exit(EXIT_FAILURE);
                    }
                    break;
                case '/':
                case '\\':
                    alive = redirect(&beam, grid);
                    break;
                default:
                    alive = 0;
                    break;
            }
        }
    }

    int count = 0;
    for (size_t y = 0U; y < grid->max_y; y++)
        for (size_t x = 0U; x < grid->max_x; x++)
            if (grid->flags[y][x] > 0)
                count++;
    printf("Part 1: %d\n", count);

    queue_destroy(q);
    grid2d_destroy(grid);

    return EXIT_SUCCESS;
}
