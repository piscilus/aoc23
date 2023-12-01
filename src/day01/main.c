/*
 * Main program for advent of code 2023 day 1.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE (1024)

/* solves part 1 independently */
int
part1(FILE* fp);

/* solves part 2 independently */
int
part2(FILE* fp);

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 1: Trebuchet?!\n\n");

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

    int p1_cv = part1(fp);
    rewind(fp);
    int p2_cv = part2(fp);
    fclose(fp);

    printf("Part 1: Calibration value = %d\n", p1_cv);
    printf("Part 2: Calibration value = %d\n", p2_cv);

    return EXIT_SUCCESS;
}

int
part1(FILE* fp)
{
    assert(fp != NULL);

    int calibration_value = 0;
    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int number = 0;
        int mul = 10;
        for (size_t i = 0; i < sizeof(line); i++)
        {
            if (line[i] == '\n')
            {
                break;
            }
            if ((line[i] >= 0x31) && (line[i] <= 0x39))
            {
                if (mul == 10)
                {
                    number = line[i] - 0x30;
                    calibration_value += number * mul;
                    mul = 0;
                }
                else
                {
                    number = line[i] - 0x30;
                }
            }
        }
        calibration_value += number;
    }

    return calibration_value;
}

int
part2(FILE* fp)
{
    assert(fp != NULL);

    const char* digits[] =
    {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    int calibration_value = 0;
    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        size_t len = 0U;
        while (line[len++] != '\n'); /* assume EOL exists */
        line[len - 1U] = '\0';
        int number = 0;
        int mul = 10;
        for (size_t i = 0; i < strlen(line); i++)
        {
            if ((line[i] >= 0x31) && (line[i] <= 0x39))
            {
                if (mul == 10)
                {
                    number = line[i] - 0x30;
                    calibration_value += number * mul;
                    mul = 0;
                }
                else
                {
                    number = line[i] - 0x30;
                }
            }
            else
            {
                for (size_t n = 0; n < 9; n++)
                {
                    if (strncmp(&line[i], digits[n], strlen(digits[n])) == 0)
                    {
                        if (mul == 10)
                        {
                            number = n + 1;
                            calibration_value += number * mul;
                            mul = 0;
                        }
                        else
                        {
                            number = n + 1;
                        }
                        break;
                    }
                }
            }
        }
        calibration_value += number;
    }

    return calibration_value;
}
