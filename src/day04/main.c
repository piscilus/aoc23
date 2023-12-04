/*
 * Main program for advent of code 2023 day 4.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <stdio.h>
#include <string.h>

int
check_line(char* l)
{
    int wn[20];

    while (*(l++) != ':'); /* skip 'Card <N>:' */
    int i = 0;
    int chars_read = 0;
    while (l != NULL && sscanf(l, " %d%n", &wn[i], &chars_read) == 1)
    {
        i++;
        l = strchr(l + chars_read, ' ');
    }
    while (*(l++) != '|');
    int num = 0;
    int num_wins = 0;
    while (l != NULL && sscanf(l, " %d%n", &num, &chars_read) == 1)
    {
        for (size_t i = 0U; i < (sizeof(wn) / sizeof(wn[0])); i++)
            if (wn[i] == num)
            {
                num_wins++;
                break;
            }
        l = strchr(l + chars_read, ' ');
    }

    if (num_wins > 0)
    {
        int res = 1;
        for (int i = 0; i < (num_wins - 1); i++)
            res *= 2;
        return res;
    }
    else
        return 0;
}

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 4: Scratchcards\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d;
    d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    char* l = NULL;
    int part1 = 0;
    while ((l = data_next_line(d)) != NULL)
        part1 += check_line(l);

    printf("Part1: %d\n", part1);

    data_destroy(d);

    return EXIT_SUCCESS;
}
