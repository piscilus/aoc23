/*
 * Main program for advent of code 2023 day 7.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "compare.h"
#include "data.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CARDS_PER_HAND (5)
#define NUM_CARD_TYPES (13)

typedef struct
{
    char hand[CARDS_PER_HAND + 1]; // null terminated
    int bid;
    int score[CARDS_PER_HAND + 1]; // rank + card values in original order
} hands_t;

// A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, 2
// 12 11 10 9  8  7  6  5  4  3  2  1  0
static int
transform(char c)
{
    if ((c >= '2') && (c <= '9'))
        return (c - '2');
    if (c == 'T')
        return 8;
    if (c == 'J')
        return 9;
    if (c == 'Q')
        return 10;
    if (c == 'K')
        return 11;
    if (c == 'A')
        return 12;
    return -1;
}

static bool
search_for(int cc[], int num, size_t qty)
{
    size_t count = 0U;
    for (size_t i = 0U; i < NUM_CARD_TYPES; i++)
        if (cc[i] == num)
            count++;
    return count == qty;
}

static int
eval(int cc[])
{
    if (search_for(cc, 5, 1U))
        return 6;
    if (search_for(cc, 4, 1U))
        return 5;
    if (search_for(cc, 3, 1U) && search_for(cc, 2, 1U))
        return 4;
    if (search_for(cc, 3, 1U))
        return 3;
    if (search_for(cc, 2, 2U))
        return 2;
    if (search_for(cc, 2, 1U))
        return 1;
    return 0;
}

static int
compare_hands(const void *a, const void *b)
{
    return memcmp(((hands_t *)a)->score, ((hands_t*)b)->score, sizeof(((hands_t*)a)->score));
}

int
main(int argc, char *argv[])
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

    size_t num_hands = data_num_lines(d);

    hands_t* array;
    array = (hands_t*)calloc(num_hands, sizeof(hands_t));

    char* l;
    size_t h = 0U;
    while ((l = data_next_line(d)) != NULL)
    {
        assert(h < num_hands);
        if (sscanf(l, "%5s %d", array[h].hand, &array[h].bid) != 2)
            exit(EXIT_FAILURE);
        array[h].hand[CARDS_PER_HAND] = '\0';
        int card_count[NUM_CARD_TYPES] = {0};
        for (size_t i = 0U; i < CARDS_PER_HAND; i++)
        {
            int t = transform(array[h].hand[i]);
            assert((t >= 0) && (t < NUM_CARD_TYPES));
            array[h].score[i + 1] = t;
            card_count[t]++;
        }
        array[h].score[0] = eval(card_count);
        h++;
    }

    qsort(array, num_hands, sizeof(array[0]), compare_hands);

    int part1 = 0;
    for (size_t i = 0U; i < num_hands; i++)
        part1 += array[i].bid * (i + 1);
    printf("Part 1: %d\n", part1);

    free(array);
    data_destroy(d);

    return EXIT_SUCCESS;
}
