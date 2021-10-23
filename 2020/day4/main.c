/* main.c (Day4)
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
#include "tokens.h"
#include "buffer.h"

static int
_get_passport(file_handle_t *file, buffer_t *buffer)
{
    buffer_reset(buffer);

    while (!file_eof(file)) {
        file_getline(file);

        if (!memcmp(file->line, "\n", 1)) {
            break;
        }

        buffer_write(buffer, file->line, strlen(file->line));
    }

    return (0);
}

static int
_hcl_valid(char *hcl)
{
    char *rest = &hcl[1];
    /* first char must be "#" */
    if (memcmp(hcl, "#", 1)) {
        return (0);
    }

    if (strlen(rest) != 6) {
        return (0);
    }

    /* next 6 must be 0-9, a-f */
    for (int i = 0;
         i < strlen(rest); i++) {
        if (rest[i] < '0' || rest[i] > '9') {
            if (rest[i] < 'a' || rest[i] > 'f') {
                return (0);
            }
        }
    }

    return (1);
}

static int
_ecl_valid(char *ecl)
{
    if (strlen(ecl) != 3) {
        return (0);
    }

    if (!memcmp(ecl, "amb", 3)) {
        return (1);
    }
    else if (!memcmp(ecl, "blu", 3)) {
        return (1);
    }
    else if (!memcmp(ecl, "brn", 3)) {
        return (1);
    }
    else if (!memcmp(ecl, "gry", 3)) {
        return (1);
    }
    else if (!memcmp(ecl, "grn", 3)) {
        return (1);
    }
    else if (!memcmp(ecl, "hzl", 3)) {
        return (1);
    }
    else if (!memcmp(ecl, "oth", 3)) {
        return (1);
    }

    return (0);
}

static int
_pid_valid(char *pid)
{
    if (strlen(pid) != 9) {
        return (0);
    }

    for (int i = 0;
         i < strlen(pid); i++) {
        if (pid[i] < '0' || pid[i] > '9') {
            return (0);
        }
    }

    return (1);
}

static int
_passport_valid(tokens_t *tokens, buffer_t *buffer)
{
#define FIELD_BYR 0x01
#define FIELD_IYR 0x02
#define FIELD_EYR 0x04
#define FIELD_HGT 0x08
#define FIELD_HCL 0x10
#define FIELD_ECL 0x20
#define FIELD_PID 0x40
#define FIELD_CID 0x80

    uint8_t field_mask = 0;
    uint8_t data_mask = 0;

    tokens_find(tokens, (char *)buffer->data,
                " :\t\r\n", tokens->max);

    for (int i = 0;
         i < tokens->used; i+=2) {
        if (!memcmp(tokens->index[i], "byr", 3)) {
            field_mask |= FIELD_BYR;

            uint16_t byr = strtoul(tokens->index[i+1], NULL, 10);
            if (byr >= 1920 && byr <= 2002) {
                data_mask |= FIELD_BYR;
            }
        }

        else if (!memcmp(tokens->index[i], "iyr", 3)) {
            field_mask |= FIELD_IYR;

            uint16_t iyr = strtoul(tokens->index[i+1], NULL, 10);
            if (iyr >= 2010 && iyr <= 2020) {
                data_mask |= FIELD_IYR;
            }
        }

        else if (!memcmp(tokens->index[i], "eyr", 3)) {
            field_mask |= FIELD_EYR;

            uint16_t eyr = strtoul(tokens->index[i+1], NULL, 10);
            if (eyr >= 2020 && eyr <= 2030) {
                data_mask |= FIELD_EYR;
            }
        }

        else if (!memcmp(tokens->index[i], "hgt", 3)) {
            field_mask |= FIELD_HGT;

            char *str;
            uint16_t hgt = strtoul(tokens->index[i+1], &str, 10);
            if (!memcmp(str, "in", 2)) {
                if (hgt >= 59 && hgt <=76) {
                    data_mask |= FIELD_HGT;
                }
            }
            else if (!memcmp(str, "cm", 2)) {
                if (hgt >= 150 && hgt <=193) {
                    data_mask |= FIELD_HGT;
                }
            }
        }

        else if (!memcmp(tokens->index[i], "hcl", 3)) {
            field_mask |= FIELD_HCL;

            if (_hcl_valid(tokens->index[i+1])) {
                data_mask |= FIELD_HCL;
            }
        }

        else if (!memcmp(tokens->index[i], "ecl", 3)) {
            field_mask |= FIELD_ECL;

            if (_ecl_valid(tokens->index[i+1])) {
                data_mask |= FIELD_ECL;
            }
        }

        else if (!memcmp(tokens->index[i], "pid", 3)) {
            field_mask |= FIELD_PID;

            if (_pid_valid(tokens->index[i+1])) {
                data_mask |= FIELD_PID;
            }
        }
    }

    if (field_mask < 0x7f ||
        data_mask  < 0x7f) {
        return (0);
    }

    return (1);
}

static int
_valid_passports(file_handle_t *file)
{
    int valid = 0;
    buffer_t buffer;
    tokens_t tokens;

    if (tokens_create(&tokens, 128)) {
        return (-1);
    }

    if (buffer_create(&buffer, BUFFER_TYPE_CHAR, 2048)) {
        return (-1);
    }

    while (!file_eof(file)) {
        _get_passport(file, &buffer);

        if (buffer.pos) {
            if (_passport_valid(&tokens, &buffer)) {
                valid++;
            }
        }
    }

    buffer_destroy(&buffer);
    tokens_destroy(&tokens);

    return (valid);
}

int
main(int argc, char **argv)
{
    int n_valid;
    file_handle_t file;

    if (argc != 2) {
        return (-1);
    }

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    n_valid = _valid_passports(&file);

    printf("Valid Passports: %d\r\n", n_valid);

    file_close(&file);

    return (0);
}
