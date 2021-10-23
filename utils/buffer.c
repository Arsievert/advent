/* buffer.c
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert1/advent
 *
 * License: MIT
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "buffer.h"

int
buffer_create(buffer_t *buffer, uint8_t type, uint32_t size)
{
    buffer->data = NULL;

    switch (type) {
    case BUFFER_TYPE_INT:
        buffer->data = (int32_t *)
            malloc(size * sizeof(int32_t));
        buffer->size = size * (sizeof(int32_t));
        break;
    case BUFFER_TYPE_UINT:
        buffer->data = (uint32_t *)
            malloc(size * sizeof(uint32_t));
        buffer->size = size * (sizeof(uint32_t));
        break;
    case BUFFER_TYPE_CHAR:
        buffer->data = (char *)
            malloc(size * sizeof(char));
        buffer->size = size * (sizeof(char));
        break;
    default:
        return (-1);
    }

    if (!buffer->data) {
        return (-1);
    }

    buffer->pos = 0;
    buffer->type = type;

    return (0);
}

int
buffer_write(buffer_t *buffer, void *data, uint32_t size)
{
    int position;
    int item_size;
    int write_size;

    if (!buffer->data) {
        return (-1);
    }

    switch (buffer->type) {
    case BUFFER_TYPE_INT:
        item_size = sizeof(int32_t);
        break;
    case BUFFER_TYPE_UINT:
        item_size = sizeof(uint32_t);
        break;
    case BUFFER_TYPE_CHAR:
        item_size = sizeof(char);
        break;
    default:
        return (-1);
    }

    /* scale write position and amount based on buffer type */
    write_size = size * item_size;
    position = buffer->pos * item_size;

    if ((position + write_size) > buffer->size) {
        return (-1);
    }

    memcpy((buffer->data + position), data, write_size);

    buffer->pos += size;

    return (0);
}

int
buffer_reset(buffer_t *buffer)
{
    buffer->pos = 0;

    switch (buffer->type) {
    case BUFFER_TYPE_INT:
        memset(buffer->data, 0,
               (buffer->size * sizeof(int32_t)));
        break;
    case BUFFER_TYPE_UINT:
        memset(buffer->data, 0,
               (buffer->size * sizeof(uint32_t)));
        break;
    case BUFFER_TYPE_CHAR:
        memset(buffer->data, 0,
               (buffer->size * sizeof(char)));
        break;
    default:
        return (-1);
    }

    return (0);
}

int
buffer_destroy(buffer_t *buffer)
{
    buffer->pos = 0;
    buffer->size = 0;
    buffer->type = 0;

    if (buffer->data) {
        free(buffer->data);
    }

    return (0);
}
