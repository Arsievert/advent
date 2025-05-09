/* main.c (Day7)
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert/advent
 *
 * License: MIT
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sort.h"
#include "file.h"
#include "tokens.h"
#include "buffer.h"

#define BAG_NAME_MAX    (100)
#define BAG_CONTENT_MAX ( 10)

typedef struct bag_content_struct {
    /* id
     * can be used to find the bag_desc */
    int id;
    /* num
     * number of bags of this id. */
    int num;
} bag_content_t;

typedef struct bag_desc_struct {
    /* id
     * unique id used to map a name to a bag description. */
    int id;
    /* name
     * description of the bag as given by the input. */
    char name[BAG_NAME_MAX];
    /* n_contents
     * number of bag_content_t this bag contains. */
    int n_contents;
    /* contents
     * id and number of bags this bag contains. */
    bag_content_t contents[BAG_CONTENT_MAX];
} bag_desc_t;

typedef struct bag_array_struct {
    /* count
     * number of bags in the array. */
    int count;
    /* array
     * collection of bag descriptions. */
    bag_desc_t array[1024];
} bag_array_t;


static int
_parse_bag_names(file_handle_t *file, bag_array_t *bags)
{
    tokens_t tokens;

    tokens_create(&tokens, 128);

    while (!file_eof(file)) {
        file_getline(file);

        tokens_find(&tokens,
                    file->line, " ,", 128);

        for (int i = 0;
             i < 3; i++) {
            /* first three tokens are always bag name. */
            strcat(bags->array[bags->count].name, tokens.index[i]);
        }

        bags->array[bags->count].id = bags->count;
        bags->count++;
    }

    tokens_destroy(&tokens);

    return (0);
}

static char *
_get_bag_name_by_id(int id, bag_array_t *bags)
{
    for (int i = 0;
         i < bags->count; i++) {
        if (bags->array[i].id == id) {
            return ((char *)bags->array[i].name);
        }
    }

    return (NULL);
}

static int
_get_bag_id_by_name(char *name, bag_array_t *bags)
{
    for (int i = 0;
         i < bags->count; i++) {
        if (strcmp(name, bags->array[i].name) == 0) {
            return bags->array[i].id;
        }
    }

    return (-1);
}

static int
_parse_bag_contents(file_handle_t *file, bag_array_t *bags)
{
    int bag_num = 0;
    char bag_name[128] = {0};
    bag_desc_t *cur;
    tokens_t tokens;

    tokens_create(&tokens, 128);

    while (!file_eof(file)) {
        file_getline(file);

        tokens_find(&tokens, file->line, " ,.", 128);

        cur = &bags->array[bag_num];
        for (int i = 4;
             i < tokens.used; i++) {
            if (atoi(tokens.index[i])) {
                /* @note (asievert)
                 * each 'content' will start with a number, then the next
                 * three tokens will be the bag name. */
                cur->contents[cur->n_contents].num = atoi(tokens.index[i]);

                strcat(bag_name, tokens.index[i+1]);
                strcat(bag_name, tokens.index[i+2]);
                strcat(bag_name, "bags"); /* always use 'bags' since 'bag' can also be present */

                cur->contents[cur->n_contents].id = _get_bag_id_by_name((char *)bag_name, bags);
                cur->n_contents++;
                memset(&bag_name, 0, 128);

                i += 3;
            }
        }

        bag_num++;
    }

    tokens_destroy(&tokens);

    return (0);
}

static int
_parse_input(file_handle_t *file, bag_array_t *bags)
{
    _parse_bag_names(file, bags);

    file_reset(file);

    _parse_bag_contents(file, bags);

    return (0);
}

static bag_desc_t *
_get_bag_by_id(int id, bag_array_t *bags)
{
    for (int i = 0;
         i < bags->count; i++) {
        if (bags->array[i].id == id) {
            return (&bags->array[i]);
        }
    }

    return NULL;
}

static int
_contains_gold_bag(bag_desc_t *bag, bag_array_t *bags, int gold_bag_id)
{
    for (int i = 0;
         i < bag->n_contents; i++) {
        if (bag->contents[i].id == gold_bag_id) {
            return (1);
        }

        bag_desc_t *next_bag = _get_bag_by_id(bag->contents[i].id, bags);
        if (_contains_gold_bag(next_bag, bags, gold_bag_id)) {
            return (1);
        }
    }

    return (0);
}

static int
_find_gold_bag(bag_array_t *bags, int gold_bag_id, int *sum)
{
    int count = 0;

    for (int i = 0;
         i < bags->count; i++) {
        if (_contains_gold_bag(&bags->array[i], bags, gold_bag_id)) {
            count++;
        }
    }

    *sum = count;

    return (0);
}


static int
_count_gold_bags(file_handle_t *file)
{
    int id;
    int sum = 0;
    bag_array_t bags = {0};

    _parse_input(file, &bags);

    id = _get_bag_id_by_name("shinygoldbags", &bags);

    _find_gold_bag(&bags, id, &sum);

    return (sum);
}

static int
_count_nested_bags(bag_array_t *bags, int bag_id)
{
    int count = 0;
    bag_desc_t *bag;

    bag = _get_bag_by_id(bag_id, bags);

    for (int i = 0;
         i < bag->n_contents; i++) {
        count += bag->contents[i].num;

        count += bag->contents[i].num *
            _count_nested_bags(bags, bag->contents[i].id);
    }

    return (count);
}

static int
_bags_inside_gold(file_handle_t *file)
{
    int n_bags;
    bag_array_t bags = {0};

    _parse_input(file, &bags);

    n_bags = _count_nested_bags(&bags, _get_bag_id_by_name("shinygoldbags", &bags));

    return (n_bags);
}

int
main(int argc, char **argv)
{
    int bags;
    file_handle_t file = {0};

    if (argc != 2) {
        return (-1);
    }

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    bags = _count_gold_bags(&file);
    printf("Shiny Gold Bags: %d\r\n", bags);

    file_reset(&file);

    bags = _bags_inside_gold(&file);
    printf("Gold Bag Holds: %d\r\n", bags);

    file_close(&file);

    return (0);
}
