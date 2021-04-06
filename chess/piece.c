#include <assert.h>

#include "piece.h"
#include "validations.h"

static node_t* get_moveable_list_or_null_king(const piece_t** board, const char* coord);
static node_t* get_moveable_list_or_null_queen(const piece_t** board, const char* coord);
static node_t* get_moveable_list_or_null_rook(const piece_t** board, const char* coord);
static node_t* get_moveable_list_or_null_bishop(const piece_t** board, const char* coord);
static node_t* get_moveable_list_or_null_knight(const piece_t** board, const char* coord);
static node_t* get_moveable_list_or_null_pawn(const piece_t** board, const char* coord);

color_t get_color(const piece_t piece)
{
	return piece & 0xc0;
}

shape_t get_shape(const piece_t piece)
{
	return piece & 0x3e;
}

int is_moved(const piece_t piece)
{
	return piece & 0x01;
}

node_t* get_moveable_list_or_null(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	node_t* moveable_list = NULL;

	switch (get_shape(piece)) {
	case SHAPE_KING:
		moveable_list = get_moveable_list_or_null_king(board, coord);
		break;
	case SHAPE_QUEEN:
		moveable_list = get_moveable_list_or_null_queen(board, coord);
		break;
	case SHAPE_ROOK:
		moveable_list = get_moveable_list_or_null_rook(board, coord);
		break;
	case SHAPE_BISHOP:
		moveable_list = get_moveable_list_or_null_bishop(board, coord);
		break;
	case SHAPE_KNIGHT:
		moveable_list = get_moveable_list_or_null_knight(board, coord);
		break;
	case SHAPE_PAWN:
		moveable_list = get_moveable_list_or_null_pawn(board, coord);
		break;
	default:
		moveable_list = NULL;
		break;
	}

	return moveable_list;
}

// TODO: return head node instead of NULL
static node_t* get_moveable_list_or_null_pawn(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	assert(get_shape(piece) == SHAPE_PAWN);

	color_t color = get_color(piece);
	int direction = color == COLOR_WHITE ? -1 : 1;

	// forward1
	size_t dx = x;
	size_t dy = y + direction;
	if (is_valid_xy(dx, dy) && board[dy][dx] != 0) {
		// TODO: add node

		// forward2
		size_t dx2 = dx;
		size_t dy2 = dy + direction;
		if (is_valid_xy(dx2, dy2) && board[dy2][dx2] != 0) {
			// TODO: add node
		}
	}

	// left-forward
	dx = x - 1;
	dy = y + direction;
	if (is_valid_xy(dx, dy) && board[dy][dx] == 0) {
		// TODO: add node
	}

	// right-forward
	dx = x + 1;
	dy = y + direction;
	if (is_valid_xy(dx, dy) && board[dy][dx] == 0) {
		// TODO: add node
	}

	return NULL;
}

// TODO: return head node instead of NULL
static node_t* get_moveable_list_or_null_knight(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	assert(get_shape(piece) == SHAPE_KNIGHT);

	// left-forward1
	size_t dx = x - 2;
	size_t dy = y - 1;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// left-forward2
	dx = x - 1;
	dy = y - 2;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// right-forward1
	dx = x + 2;
	dy = y - 1;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// right-forward1
	dx = x + 1;
	dy = y - 2;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// left-backward1
	dx = x - 2;
	dy = y + 1;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// left-backward2
	dx = x - 1;
	dy = y + 2;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// right-backward1
	dx = x + 2;
	dy = y + 1;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	// right-backward2
	dx = x + 1;
	dy = y + 2;
	if (is_valid_xy(dx, dy)) {
		// TODO: add node
	}

	return NULL;
}

// TODO: return head node instead of NULL
// TODO: deduplication
static node_t* get_moveable_list_or_null_bishop(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	assert(get_shape(piece) == SHAPE_BISHOP);

	color_t color = get_color(piece);

	// top-left
	size_t dx = x - 1;
	size_t dy = y - 1;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			//TODO: add node
			break;
		}

		--dx;
		--dy;
	}

	// bottom-right
	dx = x + 1;
	dy = y + 1;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		++dx;
		++dy;
	}

	// top-right
	dx = x + 1;
	dy = y - 1;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		++dx;
		--dy;
	}

	// bottom-left
	dx = x - 1;
	dy = y + 1;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		--dx;
		++dy;
	}

	return NULL;
}

// TODO: return head node instead of NULL
// TODO: deduplication
static node_t* get_moveable_list_or_null_rook(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	assert(get_shape(piece) == SHAPE_ROOK);

	color_t color = get_color(piece);

	// forward
	size_t dx = x;
	size_t dy = y - 1;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		--dy;
	}

	// backward
	dx = x;
	dy = y + 1;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		++dy;
	}

	// left
	dx = x - 1;
	dy = y;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		--dx;
	}

	// right
	dx = x + 1;
	dy = y;
	while (is_valid_xy(dx, dy)) {
		piece_t other_piece = board[dy][dx];
		color_t other_color = get_color(piece);

		if (other_color == 0) {
			// TODO: add node
		}
		else if (other_color == color) {
			break;
		}
		else {
			// TODO: add node
			break;
		}

		++dx;
	}

	return NULL;
}

// TODO: return head node instead of NULL
static node_t* get_moveable_list_or_null_queen(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	assert(get_shape(piece) == SHAPE_QUEEN);

	node_t* moveable_list_rook = get_moveable_list_or_null_rook(board, coord);
	node_t* moveable_list_bishop = get_moveable_list_or_null_bishop(board, coord);

	// TODO: merge lists

	return NULL;
}

// TODO: return head node instead of NULL
static node_t* get_moveable_list_or_null_king(const piece_t** board, const char* coord)
{
	assert(board != NULL);
	assert(coord != NULL);
	assert(is_valid_coord(coord));

	size_t x = translate_board_x(coord);
	size_t y = translate_board_y(coord);

	piece_t piece = board[y][x];
	assert(get_shape(piece) == SHAPE_KING);

	// TODO: calculate moveable positions

	return NULL;
}