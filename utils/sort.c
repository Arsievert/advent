/* sort.c
 *
 * Author: Austin Sievert (arsievert1@gmail.com)
 * URL:    https://github.com/arsievert/advent
 *
 * License: MIT
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int
bubble_sort_int(int *d, uint32_t sz)
{
    int temp;
    bool swap = true;

    while (swap) {
        swap = false;
        for (int i = 0;
             i < sz; i++) {
            if (d[i] > d[i+1]) {
                temp   = d[i+1];
                d[i+1] = d[i];
                d[i]   = temp;
                swap = true;
            }
        }
    }

    return (0);
}

int
bubble_sort_char(char *d, uint32_t sz)
{
    char temp;
    bool swap = true;

    while (swap) {
        swap = false;
        for (int i = 0;
             i < sz; i++) {
            if (d[i] > d[i+1]) {
                temp   = d[i+1];
                d[i+1] = d[i];
                d[i]   = temp;
                swap = true;
            }
        }
    }

    return (0);
}

int
bubble_sort_uint(uint32_t *d, uint32_t sz)
{
    uint32_t temp;
    bool swap = true;

    while (swap) {
        swap = false;
        for (int i = 0;
             i < sz; i++) {
            if (d[i] > d[i+1]) {
                temp   = d[i+1];
                d[i+1] = d[i];
                d[i]   = temp;
                swap = true;
            }
        }
    }

    return (0);
}
