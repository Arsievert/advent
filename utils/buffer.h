/* buffer.h
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert/advent
 *
 * License: MIT
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>

#define BUFFER_TYPE_INT  (0)
#define BUFFER_TYPE_UINT (1)
#define BUFFER_TYPE_CHAR (2)

typedef struct buffer_struct {
    /* pos
     * points to next free byte in the buffer */
    uint32_t pos;
    /* type
     * type pointed to by data. */
    uint8_t type;
    /* data
     * pointer to heap. */
    void *data;
    /* size
     * size of data. */
    uint32_t size;
} buffer_t;

int  buffer_create(buffer_t *, uint8_t, uint32_t);
int  buffer_write(buffer_t *, void *, uint32_t);
int  buffer_reset(buffer_t *);
int  buffer_destroy(buffer_t *);

#endif /* _BUFFER_H_ */
