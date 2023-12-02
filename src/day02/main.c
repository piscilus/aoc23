/*
 * Main program for advent of code 2023 day 2.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE (1024)

typedef struct
{
    int r;
    int g;
    int b;
} cubes_t;

bool
part1_check(const cubes_t* bag);

void
part2_check(const cubes_t* bag, cubes_t* max);

char*
search_bag(char* p, cubes_t* bag);

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 2: Cube Conundrum\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_SIZE + 1] = {0};
    int game = 1; /* assume a subsequent sequence of game IDs -> count lines */
    int sum_part1 = 0;
    int sum_part2 = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char* p = strchr(line, '\n');
        *p = '\0';
        p = line;
        while (*(p++) != ':'); /* skip 'Game <N>:' */
        cubes_t max = {0};
        bool part1_limit = true;
        while (*p != '\0')
        {
            cubes_t bag = {0};
            p = search_bag(p, &bag);
            if (!part1_check(&bag))
                part1_limit = false;
            part2_check(&bag, &max);
            p++;
        }
        if (part1_limit)
            sum_part1 += game;
        sum_part2 += (max.r * max.g * max. b);
        game++;
    }

    printf("Part 1: Sum of IDs = %d\n", sum_part1);
    printf("Part 2: Sum of powers = %d\n", sum_part2);

    fclose(fp);
    return EXIT_SUCCESS;
}

bool
part1_check(const cubes_t* bag)
{
    if (   (bag->r <= 12)
        && (bag->g <= 13)
        && (bag->b <= 14))
        return true;
    return false;
}

void
part2_check(const cubes_t* bag, cubes_t* max)
{
    if (bag->r > max->r)
        max->r = bag->r;
    if (bag->g > max->g)
        max->g = bag->g;
    if (bag->b > max->b)
        max->b = bag->b;
}

char*
search_bag(char* p, cubes_t* bag)
{
    while ((*p != '\0') && (*p != ';'))
    {
        int quantity;
        char color;
        if (sscanf(p, "%d %c", &quantity, &color) == 2)
        {
            switch (color)
            {
                case 'r':
                    bag->r = quantity;
                    break;
                case 'g':
                    bag->g = quantity;
                    break;
                case 'b':
                    bag->b = quantity;
                    break;
                default:
                    break;
            }
            while ((*p != '\0') && (*p != ';') && (*p != ','))
                p++;
        }
        else
        {
            p++;
        }
    }

    return p;
}
