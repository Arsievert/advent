/* main.c (Day8)
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
#include <stdbool.h>

#include "sort.h"
#include "file.h"
#include "tokens.h"
#include "buffer.h"

typedef struct instr_struct {
    char name[4];
    int value;
} instr_t;

typedef struct program_struct {
    instr_t *instructions;
} program_t;

static int
_parse_input(file_handle_t *file, program_t *program)
{
    int line = 0;
    tokens_t tokens;

    tokens_create(&tokens, 8);

    while (!file_eof(file)) {
        file_getline(file);

        tokens_find(&tokens, file->line, " ", 8);

        strcpy(program->instructions[line].name, tokens.index[0]);
        program->instructions[line].value = strtol(tokens.index[1], NULL, 10);

        line++;
    }

    tokens_destroy(&tokens);

    return (0);
}

static int
_acc_value_before_loop(file_handle_t *file)
{
    int acc = 0;
    int cur_instr = 0;
    int n_instr = 0;
    int ran_instr[file->n_lines];
    program_t program = {0};

    program.instructions = (instr_t *)
        malloc(file->n_lines * (sizeof(instr_t)));

    _parse_input(file, &program);

    while (1) {
        /* check for previously ran instructions. */
        for (int i = 0;
             i < n_instr; i++) {
            if (ran_instr[i] == cur_instr) {
                return (acc);
            }
        }

        ran_instr[n_instr] = cur_instr;

        if (!strcmp(program.instructions[cur_instr].name, "acc")) {
            acc += program.instructions[cur_instr].value;
            cur_instr++;
        }
        else if (!strcmp(program.instructions[cur_instr].name, "jmp")) {
            cur_instr = cur_instr + program.instructions[cur_instr].value;
        } else {
            cur_instr++;
        }

        n_instr++;
    }

    free(program.instructions);

    return (acc);
}

static int
_run_program(program_t *program, int length) {
    int acc = 0;
    int cur_instr = 0;
    int n_instr = 0;
    int ran_instr[length];

    while (1) {
        /* check for previously ran instructions. */
        for (int i = 0;
             i < n_instr; i++) {
            if (ran_instr[i] == cur_instr) {
                return (0);
            }
        }

        /* check for end of program */
        if (cur_instr == length) {
            return (acc);
        }

        ran_instr[n_instr] = cur_instr;

        if (!strcmp(program->instructions[cur_instr].name, "acc")) {
            acc += program->instructions[cur_instr].value;
            cur_instr++;
        }
        else if (!strcmp(program->instructions[cur_instr].name, "jmp")) {
            cur_instr = cur_instr + program->instructions[cur_instr].value;
        } else {
            cur_instr++;
        }

        n_instr++;
    }
    return (0);
}

static int
_replace_instr(void *name) {
    char *n = name;
    if (!strcmp(n, "nop")) {
        memcpy(name, "jmp", 3);
        return (1);
    }
    else if (!strcmp(name, "jmp")) {
        memcpy(name, "nop", 3);
        return (1);
    }

    return (0);
}

static int
_acc_value_after_replacement(file_handle_t *file)
{
    int acc = 0;
    program_t program = {0};

    program.instructions = (instr_t *)
        malloc(file->n_lines * (sizeof(instr_t)));

    _parse_input(file, &program);

    for (int i = 0;
         i < file->n_lines; i++) {
        if (_replace_instr(&program.instructions[i].name)) {
            acc = _run_program(&program, file->n_lines);
            if (acc) {
                free(program.instructions);
                return (acc);
            }
        }
        /* reset the instruction for the next run. */
        _replace_instr(&program.instructions[i].name);
    }

    free(program.instructions);

    return (-1);
}


int
main(int argc, char **argv)
{
    int acc;
    file_handle_t file = {0};

    if (argc != 2) {
        return (-1);
    }

    if (file_open(&file, argv[1], "r")) {
        return (-1);
    }

    acc = _acc_value_before_loop(&file);
    printf("Acc Before Loop: %d\r\n", acc);

    file_reset(&file);

    acc = _acc_value_after_replacement(&file);
    printf("Acc After Replacement: %d\r\n", acc);

    file_close(&file);

    return (0);
}
