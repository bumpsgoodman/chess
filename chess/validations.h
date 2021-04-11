#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include "common_defines.h"

int is_valid_coord(const char* coord);
int is_valid_x(const size_t x);
int is_valid_y(const size_t y);
int is_valid_xy(const size_t x, const size_t y);

#endif // VALIDATIONS_H