/* main.c (Day5)
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
#include "tokens.h"
#include "buffer.h"

static int
_get_seat_id(char *seat)
{
    int row = 0;
    int column = 0;

    for (int i = 0;
         i < 7; i++) {
        if (seat[i] == 'B') {
            row += ((1 << (6-i)));
        }
    }

    for (int i = 7;
         i < 10; i++) {
        if (seat[i] == 'R') {
            column += ((1 << (2-(i-7))));
        }
    }

    return ((row * 8) + column);
}

static int
_max_seat_id(file_handle_t *file)
{
    int max = 0;
    int curr;

    while (!file_eof(file)) {
        file_getline(file);

        curr = _get_seat_id(file->line);

        if (curr > max) {
            max = curr;
        }
    }

    return (max);
}

static int
_bubble_sort(uint32_t *data, int size)
{
    int temp;
    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (int i = 0;
             i < size; i++) {
            if (data[i] > data[i+1]) {
                temp = data[i+1];
                data[i+1] = data[i];
                data[i] = temp;
                swapped = true;
            }
        }
    }

    return (0);
}

static int
_my_seat_id(file_handle_t *file)
{
    uint32_t seat_id;
    buffer_t buffer;

    buffer_create(&buffer, BUFFER_TYPE_UINT, 2048);

    while (!file_eof(file)) {
        file_getline(file);

        seat_id = _get_seat_id(file->line);
        buffer_write(&buffer, (void *)&seat_id, 1);
    }

    _bubble_sort((uint32_t *)buffer.data, buffer.pos);

    uint32_t *data = buffer.data;
    for (int i = 0;
         i <= (buffer.pos); i++) {
        if ((data[i+1] - data[i]) == 2) {
            return (data[i]+1);
        }
    }

    return (0);
}

int
main(int argc, char **argv)
{
    file_handle_t file;

    if (argc != 2) {
        return (-1);
    }

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    printf("Highest ID: %d\r\n", _max_seat_id(&file));

    file_close(&file);

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    printf("My Seat: %d\r\n", _my_seat_id(&file));

    file_close(&file);

    return (0);
}
