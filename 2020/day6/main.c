/* main.c (Day6)
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert1/advent
 *
 * License: MIT
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "file.h"
#include "sort.h"
#include "tokens.h"
#include "buffer.h"

static int
_get_group(file_handle_t *file, buffer_t *buffer)
{
    while (!file_eof(file)) {
        file_getline(file);

        if (!memcmp(file->line, "\n", 1)) {
            break;
        }

        buffer_write(buffer, file->line, (strlen(file->line)));
    }

    return (0);
}

static int
_get_group_no_whitespace(file_handle_t *file, buffer_t *buffer)
{
    while (!file_eof(file)) {
        file_getline(file);

        if (!memcmp(file->line, "\n", 1)) {
            break;
        }

        buffer_write(buffer, file->line, (strlen(file->line)-1));
    }

    return (0);
}

static int
_unique_answers(buffer_t *buffer)
{
    int uniques = 0;
    char *data = buffer->data;

    bubble_sort_char((char *)data, buffer->pos-1);

    for (int i = 0;
         i < buffer->pos; i++) {
        if (data[i] != data[i+1]) {
            uniques++;
        }
    }

    return (uniques);
}

static int
_sum_uniques(file_handle_t *file)
{
    int sum = 0;
    buffer_t buffer;

    buffer_create(&buffer, BUFFER_TYPE_CHAR, 1024);

    while (!file_eof(file)) {
        buffer_reset(&buffer);

        _get_group_no_whitespace(file, &buffer);

        if (buffer.pos) {
            sum += _unique_answers(&buffer);
        }
    }

    buffer_destroy(&buffer);

    return (sum);
}

static int
_yes_to_all(buffer_t *buffer)
{
    int sum = 0;
    int found = 0;
    tokens_t tokens;
    char *data = buffer->data;

    tokens_create(&tokens, 128);
    tokens_find(&tokens, data, "\n", 128);

    /* one person corner case. */
    if (tokens.used == 1) {
        sum = strlen(tokens.index[0]);
        tokens_destroy(&tokens);
        return (sum);
    }

    for (int i = 'a'; i <='z'; i++) {
        found = 0;
        for (int j = 0; j < tokens.used; j++) {
            for (int k = 0; k < strlen(tokens.index[j]); k++) {
                if (tokens.index[j][k] == (char)i) {
                    found++;
                }
            }
        }
        if (found == tokens.used) {
            sum++;
        }
    }

    tokens_destroy(&tokens);

    return (sum);
}

static int
_all_yes(file_handle_t *file)
{
    int sum = 0;
    buffer_t buffer;

    buffer_create(&buffer, BUFFER_TYPE_CHAR, 1024);

    while (!file_eof(file)) {
        buffer_reset(&buffer);

        _get_group(file, &buffer);

        if (buffer.pos) {
            sum += _yes_to_all(&buffer);
        }
    }

    buffer_destroy(&buffer);

    return (sum);
}

int
main(int argc, char **argv)
{
    int all_yes;
    int unique_answers;
    file_handle_t file;

    if (argc != 2) {
        return (-1);
    }

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    unique_answers = _sum_uniques(&file);
    printf("Unique Answers: %d\r\n", unique_answers);

    file_close(&file);

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    all_yes = _all_yes(&file);
    printf("All Yes: %d\r\n", all_yes);

    file_close(&file);

    return (0);
}
