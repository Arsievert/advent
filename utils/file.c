/* file.c
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
#include <unistd.h>

#include "file.h"

int
file_open(file_handle_t *handle, char * file_name, char *options)
{
    if (handle->state == FILE_STATE_OPEN) {
        return (-1);
    }

    handle->line = (char *)
        malloc(FILE_LINE_SIZE * sizeof(char));

    handle->stream = NULL;
    handle->stream = fopen(file_name, options);

    if (!handle->stream) {
        return (-1);
    }

    handle->state = FILE_STATE_OPEN;

    file_getsize_bytes(handle);
    file_getsize_lines(handle);

    return (0);
}

int
file_reset(file_handle_t *handle)
{
    if (handle->state != FILE_STATE_OPEN) {
        return (-1);
    }

    fseek(handle->stream, 0, SEEK_SET);
    return (0);
}

int
file_close(file_handle_t *handle)
{
    if (handle->state != FILE_STATE_OPEN) {
        return (-1);
    }

    fflush(handle->stream);
    fclose(handle->stream);

    free(handle->line);

    handle->stream = NULL;
    handle->state = FILE_STATE_CLOSED;

    return (0);
}

int
file_eof(file_handle_t *handle)
{
    if (handle->state != FILE_STATE_OPEN) {
        return (-1);
    }

    if (ftell(handle->stream) == handle->size) {
        return (1);
    }

    return (0);
}


int
file_getsize_bytes(file_handle_t *handle)
{
    if (handle->state != FILE_STATE_OPEN) {
        return (-1);
    }

    fseek(handle->stream, 0, SEEK_END);
    handle->size = ftell(handle->stream);
    fseek(handle->stream, 0, SEEK_SET);

    return (handle->size);
}

int
file_getsize_lines(file_handle_t *handle)
{
    if (handle->state != FILE_STATE_OPEN) {
        return (-1);
    }

    handle->n_lines = 0;

    file_reset(handle);

    while (!file_eof(handle)) {
        file_getline(handle);

        handle->n_lines++;
    }

    file_reset(handle);

    return (handle->n_lines);
}

int
file_getline(file_handle_t *handle)
{
    if (!handle->line) {
        return (-1);
    }

    if (handle->state != FILE_STATE_OPEN) {
        return (-1);
    }

    if (file_eof(handle)) {
        return (-1);
    }

    memset(handle->line, 0, FILE_LINE_SIZE);

    if (!fgets(handle->line, FILE_LINE_SIZE, handle->stream)) {
        return (-1);
    }

    return (0);
}
