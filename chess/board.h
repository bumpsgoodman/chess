#ifndef BOARD_H
#define BOARD_H

void init_board(void);
void update_board(void);
void draw_board(void);

size_t translate_board_x(const char* coord);
size_t translate_board_y(const char* coord);
void translate_coord(const size_t x, const size_t y, char* out_coord);

#endif /* BOARD_H */