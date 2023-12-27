/*
 * Main program for advent of code 2023 day 5.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    uint64_t src;
    uint64_t dst;
    uint64_t rng;
} map_t;

typedef struct
{
    size_t num;
    map_t* maps;
} list_t;

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 5: If You Give A Seed A Fertilizer\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d;
    d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    size_t num_seeds = 0U;
    uint64_t* seeds = (uint64_t*)malloc(sizeof(uint64_t));
    if (seeds == NULL)
        exit(EXIT_FAILURE);

    char* l = NULL;
    if ((l = data_set_line(d, 0U)) != NULL)
    {
        l = strchr(l, ':');
        l++;
        while (*l != '\0')
        {
            uint64_t v = 0U;
            int n = 0;
            if (sscanf(l, "%"PRIu64"%n", &v, &n) == 1)
            {
                num_seeds++;
                seeds = (uint64_t*)realloc(seeds, num_seeds * sizeof(uint64_t));
                if (seeds == NULL)
                    exit(EXIT_FAILURE);
                seeds[num_seeds - 1U] = v;
                l += n;
            }
            else
            {
                l++;
            }
        }
    }

    list_t* list = (list_t*)malloc(sizeof(list_t));
    if (list == NULL)
        exit(EXIT_FAILURE);

    size_t num_lists = 0U;
    while ((l = data_next_line(d)) != NULL)
    {
        if (strlen(l) == 0U)
        {
            num_lists++;
            list = (list_t*)realloc(list, num_lists * sizeof(list_t));
            if (list == NULL)
                exit(EXIT_FAILURE);
            list[num_lists - 1U].num = 0U;
            list[num_lists - 1U].maps = NULL;
        }
        if (isdigit(*l))
        {
            assert(num_lists > 0U);
            list[num_lists - 1].maps = (map_t *)realloc(list[num_lists - 1U].maps, (list[num_lists - 1U].num + 1U) * sizeof(map_t));
            if (sscanf(l, "%"PRIu64"%"PRIu64"%"PRIu64,
                    &list[num_lists - 1U].maps[list[num_lists - 1U].num].dst,
                    &list[num_lists - 1U].maps[list[num_lists - 1U].num].src,
                    &list[num_lists - 1U].maps[list[num_lists - 1U].num].rng) != 3)
                exit(EXIT_FAILURE);
            list[num_lists - 1U].num++;
        }
    }

    uint64_t part1 = UINT64_MAX;
    for (size_t s = 0U; s < num_seeds; s++)
    {
        uint64_t value = seeds[s];
        for (size_t i = 0U; i < num_lists; i++)
        {
            for (size_t j = 0U; j < list[i].num; j++)
            {
                if (   (value >= list[i].maps[j].src)
                    && (value < list[i].maps[j].src + list[i].maps[j].rng))
                {
                    value = list[i].maps[j].dst + (value - list[i].maps[j].src);
                    break;
                }
            }
        }
        if (value < part1)
            part1 = value;
    }

    printf("Part 1: %"PRIu64"\n", part1);

    free(seeds);
    for (size_t i = 0U; i < num_lists; i++)
        free(list[i].maps);
    free(list);

    data_destroy(d);

    return EXIT_SUCCESS;
}
