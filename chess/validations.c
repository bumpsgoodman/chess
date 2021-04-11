#include <stddef.h>

#include "validations.h"

int is_valid_coord(const char* coord)
{
    return (coord[0] >= 'a' && coord[0] <= 'h'
        && coord[1] >= '1' && coord[1] <= '8');
}

int is_valid_x(const size_t x)
{
    return (x < BOARD_WIDTH);
}

int is_valid_y(const size_t y)
{
    return (y < BOARD_HEIGHT);
}

int is_valid_xy(const size_t x, const size_t y)
{
    return (is_valid_x(x) && is_valid_y(y));
}