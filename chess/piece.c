#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "piece.h"
#include "board.h"
#include "node.h"
#include "validations.h"

static node_t* get_unchecked_movable_list_or_null(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);
static node_t* get_unchecked_movable_list_or_null_king(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);
static node_t* get_unchecked_movable_list_or_null_queen(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);
static node_t* get_unchecked_movable_list_or_null_rook(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);
static node_t* get_unchecked_movable_list_or_null_bishop(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);
static node_t* get_unchecked_movable_list_or_null_knight(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);
static node_t* get_unchecked_movable_list_or_null_pawn(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y);

static void add_coord(const piece_t board[][BOARD_WIDTH], const color_t team,
    const size_t dest_x, const size_t dest_y, node_t** plist);
static void add_coord_recursion(const piece_t board[][BOARD_WIDTH], const color_t team,
    const size_t dest_x, const size_t dest_y, const int dx, const int dy, node_t** plist);

color_t get_color(const piece_t piece)
{
    return piece & COLOR_FLAG;
}

shape_t get_shape(const piece_t piece)
{
    return piece & SHAPE_FLAG;
}

int is_first_move(const piece_t piece)
{
    return !(piece & MOVE_FLAG);
}

node_t* get_movable_list_or_null(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t src_x = translate_to_board_x(coord);
    size_t src_y = translate_to_board_y(coord);

    piece_t piece = board[src_y][src_x];
    color_t color = get_color(piece);
    node_t* movable_list = get_unchecked_movable_list_or_null(board, src_x, src_y);

    // copy board
    piece_t copied_board[BOARD_HEIGHT][BOARD_WIDTH];
    memcpy(copied_board, board, BOARD_WIDTH * BOARD_HEIGHT * sizeof(piece_t));

    // remove illegal moves
    node_t* p = movable_list;
    while (p != NULL) {
        piece_t origin_piece = copied_board[p->y][p->x];
        copied_board[p->y][p->x] = piece;
        copied_board[src_y][src_x] = 0;

        node_t* deleted_node = NULL;

        for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
            for (size_t x = 0; x < BOARD_WIDTH; ++x) {
                if (get_color(copied_board[y][x]) != color) {
                    node_t* other_movable_list = get_unchecked_movable_list_or_null(copied_board, x, y);
                    node_t* q = other_movable_list;
                    while (q != NULL) {
                        piece_t other_piece = copied_board[q->y][q->x];
                        shape_t other_shape = get_shape(other_piece);
                        color_t other_color = get_color(other_piece);

                        if (other_shape == SHAPE_KING && other_color == color) {
                            deleted_node = p;
                            goto deleted;
                        }

                        q = q->next;
                    }

                    destroy_list(other_movable_list);
                }
            }
        }

    deleted:
        copied_board[src_y][src_x] = piece;
        copied_board[p->y][p->x] = origin_piece;

        p = p->next;

        delete_node(&movable_list, deleted_node);
    }

    return movable_list;
}

static node_t* get_unchecked_movable_list_or_null(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    piece_t piece = board[y][x];
    color_t color = get_color(piece);
    node_t* movable_list = NULL;

    switch (get_shape(piece)) {
    case SHAPE_KING:
        movable_list = get_unchecked_movable_list_or_null_king(board, x, y);
        break;
    case SHAPE_QUEEN:
        movable_list = get_unchecked_movable_list_or_null_queen(board, x, y);
        break;
    case SHAPE_ROOK:
        movable_list = get_unchecked_movable_list_or_null_rook(board, x, y);
        break;
    case SHAPE_BISHOP:
        movable_list = get_unchecked_movable_list_or_null_bishop(board, x, y);
        break;
    case SHAPE_KNIGHT:
        movable_list = get_unchecked_movable_list_or_null_knight(board, x, y);
        break;
    case SHAPE_PAWN:
        movable_list = get_unchecked_movable_list_or_null_pawn(board, x, y);
        break;
    default:
        movable_list = NULL;
        break;
    }

    return movable_list;
}

static node_t* get_unchecked_movable_list_or_null_king(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    assert(board != NULL);
    assert(is_valid_xy(x, y));

    piece_t piece = board[y][x];
    piece_t piece_shape = get_shape(piece);
    piece_t piece_color = get_color(piece);

    assert(piece_shape == SHAPE_KING);

    node_t* movable_list = NULL;
    add_coord(board, piece_color, x - 1, y - 1, &movable_list);   // left-forward
    add_coord(board, piece_color, x, y - 1, &movable_list);       // forward
    add_coord(board, piece_color, x + 1, y - 1, &movable_list);   // right-forward
    add_coord(board, piece_color, x - 1, y, &movable_list);       // left
    add_coord(board, piece_color, x + 1, y, &movable_list);       // right
    add_coord(board, piece_color, x - 1, y + 1, &movable_list);   // left-backward
    add_coord(board, piece_color, x, y + 1, &movable_list);       // backward
    add_coord(board, piece_color, x + 1, y + 1, &movable_list);   // right-backward

    return movable_list;
}

static node_t* get_unchecked_movable_list_or_null_queen(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    assert(board != NULL);
    assert(is_valid_xy(x, y));

    piece_t piece = board[y][x];
    piece_t piece_shape = get_shape(piece);
    piece_t piece_color = get_color(piece);

    assert(piece_shape == SHAPE_QUEEN);

    node_t* movable_list_queen = NULL;
    node_t* movable_list_rook = get_unchecked_movable_list_or_null_rook(board, x, y);
    node_t* movable_list_bishop = get_unchecked_movable_list_or_null_bishop(board, x, y);

    // merge
    if (movable_list_rook != NULL && movable_list_bishop != NULL) {
        node_t* p = movable_list_rook;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = movable_list_bishop;

        movable_list_queen = movable_list_rook;
    }
    else if (movable_list_rook != NULL) {
        movable_list_queen = movable_list_rook;
    }
    else if (movable_list_bishop != NULL) {
        movable_list_queen = movable_list_bishop;
    }

    return movable_list_queen;
}

static node_t* get_unchecked_movable_list_or_null_rook(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    assert(board != NULL);
    assert(is_valid_xy(x, y));

    piece_t piece = board[y][x];
    piece_t piece_shape = get_shape(piece);
    piece_t piece_color = get_color(piece);

    assert((piece_shape & SHAPE_ROOK) == SHAPE_ROOK);

    node_t* movable_list = NULL;
    add_coord_recursion(board, piece_color, x, y - 1, 0, -1, &movable_list); // forward
    add_coord_recursion(board, piece_color, x, y + 1, 0, 1, &movable_list); // backward
    add_coord_recursion(board, piece_color, x - 1, y, -1, 0, &movable_list); // left
    add_coord_recursion(board, piece_color, x + 1, y, 1, 0, &movable_list); // right

    return movable_list;
}

static node_t* get_unchecked_movable_list_or_null_bishop(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    assert(board != NULL);
    assert(is_valid_xy(x, y));

    piece_t piece = board[y][x];
    piece_t piece_shape = get_shape(piece);
    piece_t piece_color = get_color(piece);

    assert((piece_shape & SHAPE_BISHOP) == SHAPE_BISHOP);

    node_t* movable_list = NULL;
    add_coord_recursion(board, piece_color, x - 1, y - 1, -1, -1, &movable_list); // left-forward
    add_coord_recursion(board, piece_color, x + 1, y + 1, 1, 1, &movable_list); // right-backward
    add_coord_recursion(board, piece_color, x + 1, y - 1, 1, -1, &movable_list); // right-forward
    add_coord_recursion(board, piece_color, x - 1, y + 1, -1, 1, &movable_list); // left-backward

    return movable_list;
}

static node_t* get_unchecked_movable_list_or_null_knight(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    assert(board != NULL);
    assert(is_valid_xy(x, y));

    piece_t piece = board[y][x];
    piece_t piece_shape = get_shape(piece);
    piece_t piece_color = get_color(piece);

    assert(piece_shape == SHAPE_KNIGHT);

    node_t* movable_list = NULL;
    add_coord(board, piece_color, x - 2, y - 1, &movable_list); // left-forward1
    add_coord(board, piece_color, x - 1, y - 2, &movable_list); // left-forward2
    add_coord(board, piece_color, x + 2, y - 1, &movable_list); // right-forward1
    add_coord(board, piece_color, x + 1, y - 2, &movable_list); // right-forward2
    add_coord(board, piece_color, x - 2, y + 1, &movable_list); // left-backwoard1
    add_coord(board, piece_color, x - 1, y + 2, &movable_list); // left-backwoard2
    add_coord(board, piece_color, x + 2, y + 1, &movable_list); // right-backwoard1
    add_coord(board, piece_color, x + 1, y + 2, &movable_list); // right-backwoard2

    return movable_list;
}

static node_t* get_unchecked_movable_list_or_null_pawn(const piece_t board[][BOARD_WIDTH], const size_t x, const size_t y)
{
    assert(board != NULL);
    assert(is_valid_xy(x, y));

    piece_t piece = board[y][x];
    piece_t piece_shape = get_shape(piece);
    piece_t piece_color = get_color(piece);

    assert(piece_shape == SHAPE_PAWN);

    int direction = (piece_color == COLOR_WHITE) ? -1 : 1;

    size_t forward_y = y + direction;
    size_t forward2_y = forward_y + direction;
    size_t top_left_x = x - 1;
    size_t top_right_x = x + 1;

    node_t* movable_list = NULL;

    // forward1 
    if (is_valid_xy(x, forward_y) && board[forward_y][x] == 0) {
        insert_front(&movable_list, x, forward_y);
    }

    // forward2 
    if (is_valid_xy(x, forward2_y) && is_first_move(piece) && board[forward2_y][x] == 0) {
        insert_front(&movable_list, x, forward2_y);
    }

    // left-forward 
    color_t top_left_color = get_color(board[forward_y][top_left_x]);
    if (is_valid_xy(top_left_x, forward_y) && top_left_color != 0 && top_left_color != piece_color) {
        insert_front(&movable_list, top_left_x, forward_y);
    }

    // right-forward
    color_t top_right_color = get_color(board[forward_y][top_right_x]);
    if (is_valid_xy(top_right_x, forward_y) && top_right_color != 0 && top_right_color != piece_color) {
        insert_front(&movable_list, top_right_x, forward_y);
    }

    return movable_list;
}

static void add_coord(const piece_t board[][BOARD_WIDTH], const color_t team,
    const size_t dest_x, const size_t dest_y, node_t** plist)
{
    assert(board != NULL);
    assert(plist != NULL);

    piece_t piece = board[dest_y][dest_x];
    if (is_valid_xy(dest_x, dest_y) && get_color(piece) != team) {
        insert_front(plist, dest_x, dest_y);
    }
}

static void add_coord_recursion(const piece_t board[][BOARD_WIDTH], const color_t team,
    const size_t dest_x, const size_t dest_y, const int dx, const int dy, node_t** plist)
{
    assert(board != NULL);
    assert(plist != NULL);

    if (!is_valid_xy(dest_x, dest_y)) {
        return;
    }

    piece_t piece = board[dest_y][dest_x];
    color_t color = get_color(piece);
    if (color == 0) {
        insert_front(plist, dest_x, dest_y);
        add_coord_recursion(board, team, dest_x + dx, dest_y + dy, dx, dy, plist);
    }
    else if (color != team) {
        insert_front(plist, dest_x, dest_y);
    }
}