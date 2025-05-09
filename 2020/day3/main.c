/* main.c (Day3)
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
_input_read(buffer_t *buffer, char *fname)
{
    file_handle_t file = {0};

    file_open(&file, fname, "r");

    if (buffer_create(buffer, BUFFER_TYPE_CHAR, file.size)) {
        return (-1);
    }

    while (1) {
        if (file_eof(&file)) {
            break;
        }

        file_getline(&file);

        buffer_write(buffer, file.line, strlen(file.line)-1);
    }

    file_close(&file);

    return (0);
}

static int
_tobogan_run(buffer_t *buffer, int down, int over, int *n_trees)
{
    int index;
    int row = 0;
    int pos = 0;
    int trees = 0;
    char *course = buffer->data;

#define LINE_SIZE 31

    while (1) {
        row += down;
        pos += over;

        if (pos >= LINE_SIZE) {
            /* pos can't overflow to the next line. */
            pos -= LINE_SIZE;
        }

        index = (row *LINE_SIZE) + pos;
        if (index >= buffer->pos) {
            break;
        }

        if(course[index] == '#') {
            trees++;
        }
    }

    *n_trees = trees;

    return (0);
}

int
main(int argc, char **argv)
{
    int n_trees;
    uint32_t total_trees = 0;
    buffer_t buffer;

    if (argc != 2) {
        return (-1);
    }

    /* read file into buffer. */
    _input_read(&buffer, argv[1]);

    _tobogan_run(&buffer, 1, 3, &n_trees);
    total_trees = n_trees;

    _tobogan_run(&buffer, 1, 1, &n_trees);
    total_trees *= n_trees;
    _tobogan_run(&buffer, 1, 5, &n_trees);
    total_trees *= n_trees;
    _tobogan_run(&buffer, 1, 7, &n_trees);
    total_trees *= n_trees;
    _tobogan_run(&buffer, 2, 1, &n_trees);
    total_trees *= n_trees;


    printf("Trees Collided: %u\r\n", total_trees);

    buffer_destroy(&buffer);

    return (0);
}
