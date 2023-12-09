/*
 * Main program for advent of code 2023 day 8.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define X(a, b, c) \
    a[0] = b[0+c]; \
    a[1] = b[1+c]; \
    a[2] = b[2+c]; \
    a[3] = '\0';

typedef struct
{
    char pos[4];
    char left[4];
    char right[4];
} map_t;

size_t
search(const map_t* map, size_t size, const char* key)
{
    for (size_t i = 0U; i < size; i++)
        if (strcmp(map[i].pos, key) == 0)
            return i;
    return SIZE_MAX;
}

size_t
find_gcd(size_t a, size_t b)
{
    while (b != 0U)
    {
        size_t tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

size_t
find_lcm(size_t a, size_t b)
{
    return (a * b) / find_gcd(a, b);
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 8: Haunted Wasteland\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    assert(data_num_lines(d) > 2U);
    size_t map_entries = data_num_lines(d) - 2U;
    map_t* map = malloc(sizeof(map_t) * map_entries);
    if (map == NULL)
        exit(EXIT_FAILURE);

    char* l = data_next_line(d);
    char* dir = l;
    size_t dir_num = strlen(dir);

    (void)data_next_line(d); /* skip empty line */
    size_t j = 0U;
    while((l = data_next_line(d)) != NULL)
    {
        assert(strlen(l) >= 15U);
        X(map[j].pos, l, 0);
        X(map[j].left, l, 7);
        X(map[j].right, l, 12);
        j++;
    }

    size_t pos = search(map, map_entries, "AAA");
    size_t step = 0U;
    int found = 0;
    while(found == 0)
    {
        if (dir[step % dir_num] == 'L')
            pos = search(map, map_entries, map[pos].left);
        else
            pos = search(map, map_entries, map[pos].right);
        assert(pos < map_entries);
        step++;
        if (strcmp(map[pos].pos, "ZZZ") == 0)
            found = 1;
    }

    printf("Part 1: %zu\n", step);

    size_t count = 0U; /* number of start points */
    for (size_t i = 0U; i < map_entries; i++)
        if (map[i].pos[2] == 'A')
            count++;

    size_t* par = malloc(count * sizeof(size_t));
    if (par == NULL)
        exit(EXIT_FAILURE);

    int m = 0;
    for (size_t i = 0U; i < map_entries; i++)
        if (map[i].pos[2] == 'A')
            par[m++] = i;

    for (size_t i = 0U; i < count; i++)
    {
        step = 0;
        while (map[par[i]].pos[2] != 'Z')
        {
            if (dir[step % dir_num] == 'L')
                par[i] = search(map, map_entries, map[par[i]].left);
            else
                par[i] = search(map, map_entries, map[par[i]].right);
            assert(par[i] < map_entries);
            step++;
        }
        par[i] = step;
    }

    size_t part2 = par[0];
    for (size_t i = 0U; i < count; i++)
        part2 = find_lcm(part2, par[i]);

    printf("Part 2: %zu\n", part2);

    free(par);
    free(map);
    data_destroy(d);

    return EXIT_SUCCESS;
}
