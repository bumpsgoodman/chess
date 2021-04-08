#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "piece.h"
#include "board.h"
#include "node.h"
#include "validations.h"

static node_t* get_moveable_list_or_null_king(const piece_t board[][BOARD_WIDTH], const char* coord);
static node_t* get_moveable_list_or_null_queen(const piece_t board[][BOARD_WIDTH], const char* coord);
static node_t* get_moveable_list_or_null_rook(const piece_t board[][BOARD_WIDTH], const char* coord);
static node_t* get_moveable_list_or_null_bishop(const piece_t board[][BOARD_WIDTH], const char* coord);
static node_t* get_moveable_list_or_null_knight(const piece_t board[][BOARD_WIDTH], const char* coord);
static node_t* get_moveable_list_or_null_pawn(const piece_t board[][BOARD_WIDTH], const char* coord);

static int is_king_moveable(const piece_t board[][BOARD_WIDTH], const color_t my_color);

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

node_t* get_moveable_list_or_null(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

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

static node_t* get_moveable_list_or_null_king(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    shape_t shape = get_shape(piece);
    color_t color = get_color(piece);

    assert(shape == SHAPE_KING);

    int direction = (color == COLOR_WHITE) ? 1 : -1;

    node_t* head = NULL;

    piece_t copied_board[BOARD_HEIGHT][BOARD_WIDTH];
    memcpy(copied_board, board, BOARD_WIDTH * BOARD_HEIGHT * sizeof(piece_t));

    /* top-left */
    size_t dx = x - 1;
    size_t dy = y - 1;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* forward */
    dx = x;
    dy = y - 1;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* top-right */
    dx = x + 1;
    dy = y - 1;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* left */
    dx = x - 1;
    dy = y;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* right */
    dx = x + 1;
    dy = y;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* bottom-left */
    dx = x - 1;
    dy = y + 1;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* backward */
    dx = x;
    dy = y + 1;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    /* bottom-right */
    dx = x + 1;
    dy = y + 1;
    if (is_valid_xy(dx, dy)) {
        piece_t origin_piece = copied_board[dy][dx];
        if (get_color(origin_piece) != color) {
            copied_board[dy][dx] = piece;
            copied_board[y][x] = 0;
            if (!is_king_moveable(copied_board, color)) {
                insert_front(&head, dx, dy);
            }
            copied_board[dy][dx] = origin_piece;
            copied_board[y][x] = piece;
        }
    }

    return head;
}

static node_t* get_moveable_list_or_null_queen(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert(get_shape(piece) == SHAPE_QUEEN);

    node_t* moveable_list_rook = get_moveable_list_or_null_rook(board, coord);
    node_t* moveable_list_bishop = get_moveable_list_or_null_bishop(board, coord);

    /* merge lists */
    if (moveable_list_rook == NULL && moveable_list_bishop == NULL) {
        return NULL;
    }
    else if (moveable_list_rook == NULL && moveable_list_bishop != NULL) {
        return moveable_list_bishop;
    }
    else if (moveable_list_rook != NULL && moveable_list_bishop == NULL) {
        return moveable_list_rook;
    }
    else {
        node_t* p = moveable_list_rook;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = moveable_list_bishop;

        return moveable_list_rook;
    }
}

/* TODO: deduplication */
static node_t* get_moveable_list_or_null_rook(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert((get_shape(piece) & SHAPE_ROOK) == SHAPE_ROOK);

    color_t color = get_color(piece);

    node_t* head = NULL;

    /* forward */
    size_t dx = x;
    size_t dy = y - 1;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        --dy;
    }

    /* backward */
    dx = x;
    dy = y + 1;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        ++dy;
    }

    /* left */
    dx = x - 1;
    dy = y;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        --dx;
    }

    /* right */
    dx = x + 1;
    dy = y;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        ++dx;
    }

    return head;
}

/* TODO: deduplication */
static node_t* get_moveable_list_or_null_bishop(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert((get_shape(piece) & SHAPE_BISHOP) == SHAPE_BISHOP);

    color_t color = get_color(piece);

    node_t* head = NULL;

    /* top-left */
    size_t dx = x - 1;
    size_t dy = y - 1;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        --dx;
        --dy;
    }

    /* bottom-right */
    dx = x + 1;
    dy = y + 1;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        ++dx;
        ++dy;
    }

    /* top-right */
    dx = x + 1;
    dy = y - 1;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        ++dx;
        --dy;
    }

    /* bottom-left */
    dx = x - 1;
    dy = y + 1;
    while (is_valid_xy(dx, dy)) {
        piece_t other_piece = board[dy][dx];
        color_t other_color = get_color(other_piece);

        if (other_color == 0) {
            insert_front(&head, dx, dy);
        }
        else if (other_color == color) {
            break;
        }
        else {
            insert_front(&head, dx, dy);
            break;
        }

        --dx;
        ++dy;
    }

    return head;
}

static node_t* get_moveable_list_or_null_knight(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert(get_shape(piece) == SHAPE_KNIGHT);

    color_t color = get_color(piece);

    node_t* head = NULL;

    /* left-forward1 */
    size_t dx = x - 2;
    size_t dy = y - 1;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* left-forward2 */
    dx = x - 1;
    dy = y - 2;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* right-forward1 */
    dx = x + 2;
    dy = y - 1;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* right-forward1 */
    dx = x + 1;
    dy = y - 2;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* left-backward1 */
    dx = x - 2;
    dy = y + 1;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* left-backward2 */
    dx = x - 1;
    dy = y + 2;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* right-backward1 */
    dx = x + 2;
    dy = y + 1;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    /* right-backward2 */
    dx = x + 1;
    dy = y + 2;
    if (is_valid_xy(dx, dy) && get_color(board[dy][dx]) != color) {
        insert_front(&head, dx, dy);
    }

    return head;
}

static node_t* get_moveable_list_or_null_pawn(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert(get_shape(piece) == SHAPE_PAWN);

    color_t color = get_color(piece);
    int direction = color == COLOR_WHITE ? -1 : 1;

    node_t* head = NULL;

    /* forward1 */
    size_t dx = x;
    size_t dy = y + direction;
    if (is_valid_xy(dx, dy) && board[dy][dx] == 0) {
        insert_front(&head, dx, dy);

        /* forward2 */
        size_t dx2 = dx;
        size_t dy2 = dy + direction;
        if (is_valid_xy(dx2, dy2) && board[dy2][dx2] == 0) {
            insert_front(&head, dx2, dy2);
        }
    }

    /* left-forward */
    dx = x - 1;
    dy = y + direction;
    if (is_valid_xy(dx, dy) && board[dy][dx] != 0) {
        insert_front(&head, dx, dy);
    }

    /* right-forward */
    dx = x + 1;
    dy = y + direction;
    if (is_valid_xy(dx, dy) && board[dy][dx] != 0) {
        insert_front(&head, dx, dy);
    }

    return head;
}

static int is_king_moveable(const piece_t board[][BOARD_WIDTH], const color_t my_color)
{
    char coord[COORD_LENGTH];
    node_t* moveable_list = NULL;

    for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
        for (size_t x = 0; x < BOARD_WIDTH; ++x) {
            piece_t piece = board[y][x];
            shape_t shape = get_shape(piece);
            color_t color = get_color(piece);

            if (color != my_color) {
                if (shape == SHAPE_KING) {
                    if (is_valid_xy(x - 1, y - 1) && get_shape(board[y - 1][x - 1]) == SHAPE_KING
                            || is_valid_xy(x, y - 1) && get_shape(board[y - 1][x]) == SHAPE_KING
                            || is_valid_xy(x + 1, y - 1) && get_shape(board[y - 1][x + 1]) == SHAPE_KING
                            || is_valid_xy(x - 1, y) && get_shape(board[y][x - 1]) == SHAPE_KING
                            || is_valid_xy(x + 1, y) && get_shape(board[y][x + 1]) == SHAPE_KING
                            || is_valid_xy(x - 1, y + 1) && get_shape(board[y + 1][x - 1]) == SHAPE_KING
                            || is_valid_xy(x, y + 1) && get_shape(board[y + 1][x]) == SHAPE_KING
                            || is_valid_xy(x + 1, y + 1) && get_shape(board[y + 1][x + 1]) == SHAPE_KING) {
                        return TRUE;
                    }

                    continue;
                }

                translate_to_coord(x, y, coord);
                node_t* moveable_list = get_moveable_list_or_null(board, coord);
                node_t* p = moveable_list;
                
                while (p != NULL) {
                    piece_t target = board[p->y][p->x];
                    shape_t target_shape = get_shape(target);
                    color_t target_color = get_color(target);
                    if (target_shape == SHAPE_KING && target_color == my_color) {
                        destroy_node(p);
                        return TRUE;
                    }

                    p = p->next;
                }

                destroy_node(p);
            }
        }
    }

    return FALSE;
}