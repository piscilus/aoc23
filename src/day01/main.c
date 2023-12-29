/*
 * Main program for advent of code 2023 day 1.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
part1(data_t* d)
{
    int calibration_value = 0;
    char* l;
    while ((l = data_next_line(d)) != NULL)
    {
        int number = 0;
        int mul = 10;
        for (size_t i = 0U; i < strlen(l); i++)
        {
            if ((l[i] >= 0x31) && (l[i] <= 0x39)) /* digit 0..9 */
            {
                if (mul == 10) /* the first digit */
                {
                    number = l[i] - 0x30;
                    calibration_value += number * mul;
                    mul = 0;
                }
                else /* any other digit -> the last */
                {
                    number = l[i] - 0x30;
                }
            }
        }
        calibration_value += number;
    }

    return calibration_value;
}

static int
part2(data_t* d)
{
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
    char* l;
    while ((l = data_next_line(d)) != NULL)
    {
        int number = 0;
        int mul = 10;
        for (size_t i = 0U; i < strlen(l); i++)
        {
            if ((l[i] >= 0x31) && (l[i] <= 0x39)) /* digit 0..9 */
            {
                if (mul == 10) /* the first digit */
                {
                    number = l[i] - 0x30;
                    calibration_value += number * mul;
                    mul = 0;
                }
                else /* any other digit -> the last */
                {
                    number = l[i] - 0x30;
                }
            }
            else
            {
                /* one of the spelled numbers? */
                for (size_t n = 0U; n < 9U; n++)
                {
                    if (strncmp(&l[i], digits[n], strlen(digits[n])) == 0)
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

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 1: Trebuchet?!\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    int calibration_value;

    calibration_value = part1(d);
    printf("Part 1: Calibration value = %d\n", calibration_value);

    data_reset(d);
    calibration_value = part2(d);
    printf("Part 2: Calibration value = %d\n", calibration_value);

    data_destroy(d);

    return EXIT_SUCCESS;
}
