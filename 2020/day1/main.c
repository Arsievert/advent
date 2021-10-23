/* main.c (Day1)
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

#include "file.h"
#include "buffer.h"

static int
parse_input(buffer_t *buffer, char *file_name)
{
    int32_t data;
    file_handle_t file;

    file_open(&file, file_name, "r");
    file_getsize(&file);

    while (1) {
        if (file_eof(&file)) {
            break;
        }

        file_getline(&file);

        /* convert line to int, store in buffer. */
        data = strtol(file.line, NULL, 10);
        buffer_write(buffer, (void *)&data, 1);
    }

    file_close(&file);

    return (0);
}

static int
sum2_2020(int32_t *data, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (data[i] + data[j] == 2020) {
                printf("%d * %d = %d\r\n",
                       data[i], data[j],
                       (data[i] * data[j]));
                return (0);
            }
        }
    }

    return (-1);
}

static int
sum3_2020(int32_t *data, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                if (data[i] + data[j] + data[k] == 2020) {
                    printf("%d * %d * %d = %d\r\n",
                           data[i], data[j], data[k],
                           (data[i] * data[j] * data[k]));
                    return (0);
                }
            }
        }
    }

    return (-1);
}

int
main(int argc, char **argv)
{
    buffer_t buffer;

    if (argc != 2) {
        return (-1);
    }

    buffer_create(&buffer, BUFFER_TYPE_INT, 1000);

    parse_input(&buffer, argv[1]);
    sum2_2020((int32_t *)buffer.data, buffer.pos);
    sum3_2020((int32_t *)buffer.data, buffer.pos);

    buffer_destroy(&buffer);

    return (0);
}
