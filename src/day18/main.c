/*
 * Main program for advent of code 2023 day 18.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"
#include "queue.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RGB_NUM_BYTES (3)
#define FILL ('O')

typedef struct
{
    int min;
    int max;
} range_t;

typedef struct
{
    int x;
    int y;
} coord_t;

typedef struct
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
} color_t;

typedef struct
{
    color_t color;
    coord_t from;
    coord_t to;
    char dir;
} trench_t;

static void
flood_fill(char** grid, coord_t grid_size, int x, int y, char find, char fill)
{
    if (   (x < 0) || (x >= grid_size.x)
        || (y < 0) || (y >= grid_size.y)
        || (grid[y][x] != find))
        return;

    grid[y][x] = fill;

    flood_fill(grid, grid_size, x + 1, y,     find, fill);
    flood_fill(grid, grid_size, x - 1, y,     find, fill);
    flood_fill(grid, grid_size, x,     y + 1, find, fill);
    flood_fill(grid, grid_size, x,     y - 1, find, fill);
}

static void
set_pixel(unsigned char* img, int pos, const color_t* color)
{
    int index = pos * RGB_NUM_BYTES;
    img[index + 0] = color->r;
    img[index + 1] = color->g;
    img[index + 2] = color->b;
}

int
main(int argc, char* argv[])
{
    printf("Advent of Code 2023 - Day 18: Lavaduct Lagoon\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    queue_t* q = queue_init(sizeof(trench_t));
    if (q == NULL)
        exit(EXIT_FAILURE);

    range_t x = {0};
    range_t y = {0};
    coord_t pos = {0};
    char* l;
    while((l = data_next_line(d)) != NULL)
    {
        trench_t t;
        int length = 0;
        if (sscanf(l, "%c %d (#%2x%2x%2x)", &t.dir, &length, &t.color.r, &t.color.g, &t.color.b) == 5)
        {
            t.from = pos;
            switch(t.dir)
            {
                case 'R':
                    pos.x += length;
                    if (pos.x > x.max)
                        x.max = pos.x;
                    break;
                case 'L':
                    pos.x -= length;
                    if (pos.x < x.min)
                        x.min = pos.x;
                    break;
                case 'U':
                    pos.y -= length;
                    if (pos.y < y.min)
                        y.min = pos.y;
                    break;
                case 'D':
                    pos.y += length;
                    if (pos.y > y.max)
                        y.max = pos.y;
                    break;
                default:
                    break;
            }
            t.to = pos;
            queue_enqueue(q, &t);
        }
    }

    coord_t grid_size = { .y = y.max - y.min + 1, .x = x.max - x.min + 1 };
    char** grid = (char**)malloc(grid_size.y * sizeof(char*));
    for (int i = 0; i <grid_size.y; ++i)
        grid[i] = (char*)malloc(grid_size.x * sizeof(char));

    for (int i = 0; i < grid_size.y; ++i)
        for (int j = 0; j < grid_size.x; ++j)
            grid[i][j] = '.';

    unsigned char* img = (unsigned char*)malloc(grid_size.x * grid_size.y * RGB_NUM_BYTES);
    memset(img, 0x2F, grid_size.x * grid_size.y * RGB_NUM_BYTES);

    trench_t t;
    while(queue_dequeue(q, &t))
    {
        switch(t.dir)
        {
            case 'R':
                for (int i = t.from.x; i <= t.to.x; i++)
                {
                    grid[t.from.y - y.min][i - x.min] = '#';
                    set_pixel(img, (t.from.y - y.min) * grid_size.x + (i - x.min), &t.color);
                }
                break;
            case 'L':
                for (int i = t.from.x; i > t.to.x; i--)
                {
                    grid[t.from.y - y.min][i - x.min] = '#';
                    set_pixel(img, (t.from.y - y.min) * grid_size.x + (i - x.min), &t.color);
                }
                break;
            case 'D':
                for (int i = t.from.y; i <= t.to.y; i++)
                {
                    grid[i - y.min][t.from.x - x.min] = '#';
                    set_pixel(img, (i - y.min) * grid_size.x + (t.from.x - x.min), &t.color);
                }
                break;
            case 'U':
                for (int i = t.from.y; i > t.to.y; i--)
                {
                    grid[i - y.min][t.from.x - x.min] = '#';
                    set_pixel(img, (i - y.min) * grid_size.x + (t.from.x - x.min), &t.color);
                }
                break;
            default:
                break;
        }
    }

    /* TODO: determine start point within figure */
    flood_fill(grid, grid_size, grid_size.x / 2, grid_size.y / 2, '.', FILL);

    color_t fill = { 0x8f, 0x8f, 0x8f};
    for (int y = 0; y < grid_size.y; ++y)
        for (int x = 0; x < grid_size.x; ++x)
            if (grid[y][x] == FILL)
                set_pixel(img, y * grid_size.x + x, &fill);

    stbi_write_png("map.png", grid_size.x, grid_size.y, RGB_NUM_BYTES, img, grid_size.x * RGB_NUM_BYTES);
    free(img);

    int count = 0;
    for (int y = 0; y < grid_size.y; y++)
        for (int x = 0; x < grid_size.x; x++)
            if ((grid[y][x] == '#') || (grid[y][x] == FILL))
                count++;

    printf("Part 1: %d\n", count);

    for (int i = 0; i < x.max; ++i)
        free(grid[i]);
    free(grid);

    queue_destroy(q);
    data_destroy(d);

    return EXIT_SUCCESS;
}
