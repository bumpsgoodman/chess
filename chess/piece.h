#ifndef PIECE_H
#define PIECE_H

#include "common_defines.h"
#include "node.h"

#define COLOR_FLAG (0xc0)
#define SHAPE_FLAG (0x3e)
#define MOVE_FLAG (0x1)

typedef unsigned char piece_t;

typedef enum shape {
	SHAPE_PAWN      = (1 << 1),
	SHAPE_KNIGHT    = (1 << 2),
	SHAPE_BISHOP    = (1 << 3),
	SHAPE_ROOK      = (1 << 4),
	SHAPE_QUEEN     = (SHAPE_ROOK | SHAPE_BISHOP),
	SHAPE_KING      = (1 << 5)
} shape_t;

typedef enum color {
	COLOR_BLACK = (1 << 6),
	COLOR_WHITE = (1 << 7)
} color_t;

color_t get_color(const piece_t piece);
shape_t get_shape(const piece_t piece);
int is_first_move(const piece_t piece);

node_t* get_moveable_list_or_null(const piece_t board[][BOARD_WIDTH], const char* coord);

#endif /* PIECE_H */