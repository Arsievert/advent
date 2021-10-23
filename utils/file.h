/* file.h
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert1/advent
 *
 * License: MIT
 */

#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdint.h>

#define FILE_STATE_NONE   (0)
#define FILE_STATE_OPEN   (1)
#define FILE_STATE_CLOSED (2)

#define FILE_LINE_SIZE    (256)

typedef struct file_handle_struct {
    /* state
     * current status of the file */
    uint8_t state;
    /* stream
     * pointer to opened file. */
    FILE *stream;
    /* size
     * file size in bytes */
    uint32_t size;
    /* n_lines
     * number of lines contained in the file. */
    uint32_t n_lines;
    /* line
     * pointer to the last read line from the file. */
    char *line;
} file_handle_t;

int  file_open(file_handle_t *, char *, char *);
int  file_reset(file_handle_t *);
int  file_close(file_handle_t *);
int  file_eof(file_handle_t *);
int  file_getsize_bytes(file_handle_t *);
int  file_getsize_lines(file_handle_t *);
int  file_getline(file_handle_t *);

#endif /* _FILE_H_ */
