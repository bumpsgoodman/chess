#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "validations.h"

// #define REDIRECTION_MODE

char g_src_coord[COORD_LENGTH];
char g_dest_coord[COORD_LENGTH];

static int input_coord(char* coord);

void input(void)
{
    printf("from coordinates\n> ");
    if (input_coord(g_src_coord) == FALSE) {
        fprintf(stderr, "failed read data");
        assert(FALSE && "failed read data");
    }

    printf("to coordinates\n> ");
    if (input_coord(g_dest_coord) == FALSE) {
        fprintf(stderr, "failed read data");
        assert(FALSE && "failed read data");
    }
}

static int input_coord(char* coord)
{
    assert(coord != NULL);

    char line[COORD_LENGTH];
    while (TRUE) {
        if (fgets(line, COORD_LENGTH, stdin) == NULL) {
            clearerr(stdin);
            return FALSE;
        }

        if (sscanf(line, "%s", coord) == 1) {
            if (is_valid_coord(coord)) {
#ifndef REDIRECTION_MODE
                rewind(stdin);
#endif // REDIRECTION_MODE
                return TRUE;
            }
        }
    }
}