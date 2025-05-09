/* tokens.c
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert/advent
 *
 * License: MIT
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tokens.h"

int
tokens_create(tokens_t *tokens, uint16_t n)
{
    tokens->max = 0;
    tokens->used = 0;

    tokens->index = (char **)
        malloc(n * (sizeof(char *)));

    if (tokens->index) {
        tokens->max = n;
        return (0);
    }

    return (-1);
}

static int
_is_delimeter(char s, char *d)
{
    int len;

    if (!d) {
        return (0);
    }

    len = strlen(d);

    for (int i = 0; i < len; i++) {
        if (d[i] == s) {
            return (1);
        }
    }

    return (0);
}

int
tokens_find(tokens_t *tokens, char *input, char *delimeters, uint16_t n_tokens)
{
    int len;
    bool new_token;

    if ((!input) ||
        (!delimeters) ||
        (!n_tokens) ||
        (!tokens->index)) {
        return (-1);
    }

    if (n_tokens > tokens->max) {
        return (-1);
    }

    tokens->used = 0;

    new_token = true;

    /* Use the length of the string before mutilation. */
    len = strlen(input);

    for (int i = 0; i < len; i++) {
        if (_is_delimeter(input[i], delimeters)) {
            input[i] = 0;
            new_token = true;
            continue;
        }

        if (new_token) {
            tokens->index[tokens->used++] = (char *)&input[i];
            new_token = false;
        }

        if (tokens->used == n_tokens ||
            tokens->used == tokens->max) {
            return (0);
        }
    }

    return (0);
}

int
tokens_destroy(tokens_t *tokens)
{
    if (tokens->index) {
        free(tokens->index);
    }

    tokens->used = 0;
    tokens->max = 0;

    return (0);
}
