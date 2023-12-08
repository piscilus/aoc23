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
check_line(char* l, size_t* wins)
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
    *wins = 0U;
    while (l != NULL && sscanf(l, " %d%n", &num, &chars_read) == 1)
    {
        for (size_t i = 0U; i < (sizeof(wn) / sizeof(wn[0])); i++)
            if (wn[i] == num)
            {
                (*wins)++;
                break;
            }
        l = strchr(l + chars_read, ' ');
    }

    if (*wins > 0U)
    {
        int res = 1;
        for (size_t i = 0U; i < (*wins - 1U); i++)
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

    int part1 = 0;
    int part2 = 0;

    size_t num_cards = data_num_lines(d);
    int* a = calloc(num_cards, sizeof(int));
    if (a == NULL)
        return EXIT_FAILURE;

    size_t wins;
    char* l = NULL;
    size_t card = 0;
    while ((l = data_next_line(d)) != NULL)
    {
        part1 += check_line(l, &wins);
        a[card]++;
        for (int c = 0; c < a[card]; c++)
            for (size_t n = card + 1U; n < card + 1U + wins; n++)
                if (n < num_cards)
                    a[n]++;
        card++;
    }

    for(size_t i = 0U; i < num_cards; i++)
        part2 += a[i];

    printf("Part1: %d\n", part1);
    printf("Part2: %d\n", part2);

    free(a);

    data_destroy(d);

    return EXIT_SUCCESS;
}
