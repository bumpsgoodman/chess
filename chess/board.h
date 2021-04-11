#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "common_defines.h"

void init_board(void);
void update_board(void);
void draw_board(void);

int is_check(const piece_t board[][BOARD_WIDTH], const color_t team);

size_t translate_to_board_x(const char* coord);
size_t translate_to_board_y(const char* coord);
void translate_to_coord(const size_t x, const size_t y, char* out_coord);

#endif /* BOARD_H */