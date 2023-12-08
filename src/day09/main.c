/*
 * Main program for advent of code 2023 day 9.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_PER_LINE (30U)

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 9: Mirage Maintenance\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    int part1 = 0;
    int part2 = 0;

    char* l;
    while ((l = data_next_line(d)) != NULL)
    {
        int a[MAX_NUM_PER_LINE];
        size_t n = 0U;
        int chars_read = 0;
        while ((l != NULL) && (sscanf(l, " %d%n", &a[n], &chars_read) == 1))
        {
            l += chars_read;
            n++;
            assert(n < MAX_NUM_PER_LINE);
        }

        int first[MAX_NUM_PER_LINE];
        int last[MAX_NUM_PER_LINE];
        int diff_total = 1;
        size_t j = 1U;
        while ((diff_total != 0) && (j < n))
        {
            diff_total = 0;
            first[j - 1U] = a[0];
            last[j - 1U] = a[n - j];
            for (size_t i = 0U; i < (n - j); i++)
            {
                a[i] = a[i + 1U] - a[i];
                diff_total += a[i];
            }
            j++;
        }
        if ((j == n) && (diff_total != 0)) /* sanity check */
        {
            printf("Last line not 0 but %d!\n", diff_total);
            exit(EXIT_FAILURE);
        }
        int history_first = 0;
        int history_last = 0;
        assert(j > 0U);
        for (size_t i = (j - 1U); i != 0U; --i)
        {
            history_first = first[i - 1U] - history_first;
            history_last += last[i - 1U];
        }
        part1 += history_last;
        part2 += history_first;
    }

    printf("Part1 %d\n", part1);
    printf("Part2 %d\n", part2);

    data_destroy(d);

    return EXIT_SUCCESS;
}
