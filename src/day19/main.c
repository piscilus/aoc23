/*
 * Main program for advent of code 2023 day 19.
 *
 * (C) Copyright 2023 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "da1d.h"
#include "data.h"
#include "hashtable.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define WORKFLOW_NAME_LEN (3 + 1)
#define MAX_RULES_PER_WORKFLOW (5)

typedef enum
{
    CAT_X = 0,
    CAT_M,
    CAT_A,
    CAT_S,
    NUM_CAT_
} category_t;

typedef struct
{
    category_t cat;
    char compare;
    int value;
    char w_name[WORKFLOW_NAME_LEN];
} rule_t;

typedef struct
{
    size_t num_rules;
    rule_t rules[MAX_RULES_PER_WORKFLOW];
} workflow_t;

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2023 - Day 19: Aplenty\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name!\n");
        exit(EXIT_FAILURE);
    }

    data_t* d = data_init(argv[1]);
    if (d == NULL)
        exit(EXIT_FAILURE);

    hashtable_t* ht = ht_init(data_num_lines(d));
    if (ht == NULL)
        exit(EXIT_FAILURE);

    char* l;
    while((l = data_next_line(d)) != NULL)
    {
        if (strlen(l) > 0U)
        {
            char* t;
            t = strtok(l, "{},");
            char* wfn = t;
            workflow_t wf = {0};
            while((t = strtok(NULL, "{},")) != NULL)
            {
                char cat;
                if (sscanf(t, "%c%c%d:%3s", &cat, &wf.rules[wf.num_rules].compare, &wf.rules[wf.num_rules].value, wf.rules[wf.num_rules].w_name) == 4)
                {
                    switch (cat)
                    {
                        case 'x':
                            wf.rules[wf.num_rules].cat = CAT_X;
                            break;
                        case 'm':
                            wf.rules[wf.num_rules].cat = CAT_M;
                            break;
                        case 'a':
                            wf.rules[wf.num_rules].cat = CAT_A;
                            break;
                        case 's':
                            wf.rules[wf.num_rules].cat = CAT_S;
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    wf.rules[wf.num_rules].cat = NUM_CAT_;
                    memcpy(wf.rules[wf.num_rules].w_name, t, WORKFLOW_NAME_LEN);
                }
                wf.num_rules++;
            }
            if (ht_insert(ht, wfn, &wf, sizeof(workflow_t)) == NULL)
                assert(0);
        }
        else
            break;
    }

    size_t num_parts = 0U;
    da1d_t* da = da1d_init(data_num_lines(d), NUM_CAT_ * sizeof(int));
    if (da == NULL)
        exit(EXIT_FAILURE);

    while((l = data_next_line(d)) != NULL)
    {
        int c[NUM_CAT_];
        if (sscanf(l, "{x=%d,m=%d,a=%d,s=%d}", &c[0], &c[1], &c[2], &c[3]) == 4)
        {
            if (da1d_set(da, num_parts, c) == NULL)
                assert(0);
            num_parts++;
        }
        else
            assert(0);
    }
    if (da1d_resize(da, num_parts) == NULL)
        assert(0);

    size_t part1 = 0U;
    for (size_t i = 0U; i < da1d_get_num(da); i++)
    {
        const workflow_t* wf = ht_search(ht, "in", NULL);
        if (wf == NULL)
            exit(EXIT_FAILURE);
        int* c = da1d_get(da, i);
        if (c == NULL)
            assert(0);
        int run = 1;
        while (run)
        {
            const char* wfn;
            for (size_t r = 0U; r < wf->num_rules; r++)
            {
                if (wf->rules[r].cat < NUM_CAT_)
                {
                    if (   (wf->rules[r].compare == '>')
                        && (c[wf->rules[r].cat] > wf->rules[r].value))
                    {
                        wfn = wf->rules[r].w_name;
                        break;
                    }
                    else if (  (wf->rules[r].compare == '<')
                            && (c[wf->rules[r].cat] < wf->rules[r].value))
                    {
                        wfn = wf->rules[r].w_name;
                        break;
                    }
                    /* else: next rule */
                }
                else
                {
                    wfn = wf->rules[r].w_name;
                    break;
                }
            }
            if (*wfn == 'A')
            {
                part1 += c[0] + c[1] + c[2] + c[3];
                run = 0;
            }
            else if (*wfn == 'R')
            {
                run = 0;
            }
            else
            {
                wf = ht_search(ht, wfn, NULL);
                if (wf == NULL)
                    assert(0);
            }
        }
    }

    printf("Part 1: %zu\n", part1);

    da1d_destroy(da);
    ht_destroy(ht);
    data_destroy(d);

    return EXIT_SUCCESS;
}
