/*
 * Main program for advent of code 2023 day 7.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

size_t
get_nums(const char* p, int val[], size_t size)
{
    size_t i = 0;
    int chars_read = 0;
    while (*(p++) != ':')
        ; /* skip until : */
    while ((p != NULL) && (i < size)
           && (sscanf(p, " %d%n", &val[i], &chars_read) == 1))
    {
        p = strchr(p + chars_read, ' ');
        i++;
    }

    return i;
}

uint64_t
get_num_part2(const char* p)
{
    uint64_t result = 0U;
    while (*(p++) != ':')
        ;
    while (*p)
    {
        if (isdigit(*p))
            result = (result * 10U) + (*p - '0');
        else if (*p != ' ')
            break;
        p++;
    }

    return result;
}

int
main(int argc, char* argv[])
{
    printf("Advent of Code 2023 - Day 7: Camel Cards\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d;
    d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    int tim[10];
    int dis[10];
    size_t num_tim =
        get_nums(data_set_line(d, 0), tim, sizeof(tim) / sizeof(tim[0]));
    size_t num_dis =
        get_nums(data_set_line(d, 1), dis, sizeof(dis) / sizeof(dis[0]));

    assert(num_dis == num_tim);

    int part1 = 1;
    for (size_t n = 0U; n < num_tim; n++)
    {
        int count = 0;
        for (int t = 1; t < tim[n]; t++)
            if ((tim[n] - t) * t > dis[n])
                count++;
        part1 *= count;
    }

    printf("Part 1: %d\n", part1);

    uint64_t time = get_num_part2(data_set_line(d, 0));
    uint64_t distance = get_num_part2(data_set_line(d, 1));

    uint64_t c = 0U;
    for (uint64_t t = 1U; t < time; t++)
        if (((time - t) * t) > distance)
            c++;
    printf("Part 2: %lu\n", c);

    data_destroy(d);

    return EXIT_SUCCESS;
}
