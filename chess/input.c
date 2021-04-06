#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>

#include "input.h"
#include "validations.h"

char g_from_coord[COORD_LENGTH];
char g_to_coord[COORD_LENGTH];

static int input_coord(char* coord);

void input(void)
{
	printf("from coordinates\n> ");
	if (input_coord(g_from_coord) == FALSE) {
		fprintf(stderr, "failed read data");
		input();
	}

	printf("to coordinates\n> ");
	if (input_coord(g_to_coord) == FALSE) {
		fprintf(stderr, "failed read data");
		input();
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
				return TRUE;
			}
		}
	}
}