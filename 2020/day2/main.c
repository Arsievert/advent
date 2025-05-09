/* main.c (Day2)
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

#include "file.h"
#include "tokens.h"
#include "buffer.h"

static int
_pass_is_valid_pt1(tokens_t *tokens)
{
    int n_times = 0;
    int min;
    int max;
    char *letter;
    char *passphrase;

    if (tokens->used != 4) {
        return (0);
    }

    /* First token: Min <
     * Second token: Max >
     * Third token: letter
     * Fourth token: passphrase. */
    min = strtoul(tokens->index[0], NULL, 10);
    max = strtoul(tokens->index[1], NULL, 10);
    letter = tokens->index[2];
    passphrase = tokens->index[3];

    for (int i = 0;
         i < strlen(passphrase); i++) {
        if (passphrase[i] == (char)letter[0]) {
            n_times++;
        }
    }

    if (n_times >= min &&
        n_times <= max) {
        return (1);
    }

    return (0);
}

static int
_pass_is_valid_pt2(tokens_t *tokens)
{
    int n_times = 0;
    int pos1;
    int pos2;
    char *letter;
    char *passphrase;

    if (tokens->used != 4) {
        return (0);
    }

    /* First token: pos1
     * Second token: pos2
     * Third token: letter
     * Fourth token: passphrase. */
    pos1 = strtoul(tokens->index[0], NULL, 10);
    pos2 = strtoul(tokens->index[1], NULL, 10);
    letter = tokens->index[2];
    passphrase = tokens->index[3];

    /* positions are 1 indexed. */
    pos1--;
    pos2--;

    if (passphrase[pos1] == (char)letter[0]) {
        n_times++;
    }

    if (passphrase[pos2] == (char)letter[0]) {
        n_times++;
    }

    if (n_times == 1) {
        return (1);
    }

    return (0);
}

int
main(int argc, char **argv)
{
    tokens_t tokens;
    file_handle_t file = {0};

    int n_valid_part1 = 0;
    int n_valid_part2 = 0;

    if (argc != 2) {
        return (-1);
    }

    if (tokens_create(&tokens, 64)) {
        return (-1);
    }

    file_open(&file, argv[1], "r");
    file_getsize_lines(&file);

    while (1) {
        if (file_eof(&file)) {
            break;
        }

        file_getline(&file);

        tokens_find(&tokens, file.line, " -:\r\n", 64);

        if (_pass_is_valid_pt1(&tokens)) {
            n_valid_part1++;
        }

        if (_pass_is_valid_pt2(&tokens)) {
            n_valid_part2++;
        }
    }

    printf("Part 1: %d\r\n", n_valid_part1);
    printf("Part 2: %d\r\n", n_valid_part2);

    file_close(&file);
    tokens_destroy(&tokens);

    return (0);
}
