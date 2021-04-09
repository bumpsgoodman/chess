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

static void add_moveable_coord_recursion(const piece_t board[][BOARD_WIDTH], const color_t color,
    const size_t x, const size_t y, const int dx, const int dy, node_t** phead);

static int is_king_moveable(const piece_t board[][BOARD_WIDTH], const size_t src_x, const size_t src_y, const int dx, const int dy);

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

    node_t* head = NULL;

    /* top-left */
    size_t dx = -1;
    size_t dy = -1;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* forward */
    dx = 0;
    dy = -1;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* top-right */
    dx = 1;
    dy = -1;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* left */
    dx = -1;
    dy = 0;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* right */
    dx = 1;
    dy = 0;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* bottom-left */
    dx = -1;
    dy = 1;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* backward */
    dx = 0;
    dy = 1;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
    }

    /* bottom-right */
    dx = 1;
    dy = 1;
    if (is_king_moveable(board, x, y, dx, dy)) {
        insert_front(&head, x + dx, y + dy);
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

static node_t* get_moveable_list_or_null_rook(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert((piece & SHAPE_ROOK) == SHAPE_ROOK);

    color_t color = get_color(piece);
    node_t* head = NULL;
    add_moveable_coord_recursion(board, color, x - 1, y, -1, 0, &head); /* left */
    add_moveable_coord_recursion(board, color, x + 1, y, 1, 0, &head); /* right */
    add_moveable_coord_recursion(board, color, x, y - 1, 0, -1, &head); /* foward */
    add_moveable_coord_recursion(board, color, x, y + 1, 0, 1, &head); /* backward */

    return head;
}

static node_t* get_moveable_list_or_null_bishop(const piece_t board[][BOARD_WIDTH], const char* coord)
{
    assert(board != NULL);
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    size_t x = translate_to_board_x(coord);
    size_t y = translate_to_board_y(coord);

    piece_t piece = board[y][x];
    assert((piece & SHAPE_BISHOP) == SHAPE_BISHOP);

    color_t color = get_color(piece);
    node_t* head = NULL;
    add_moveable_coord_recursion(board, color, x - 1, y - 1, -1, -1, &head); /* top-left */
    add_moveable_coord_recursion(board, color, x + 1, y + 1, 1, 1, &head); /* bottom-right */
    add_moveable_coord_recursion(board, color, x + 1, y - 1, 1, -1, &head); /* top-right */
    add_moveable_coord_recursion(board, color, x - 1, y + 1, -1, 1, &head); /* bottom-left */

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
    size_t next_x = x - 2;
    size_t next_y = y - 1;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* left-forward2 */
    next_x = x - 1;
    next_y = y - 2;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* right-forward1 */
    next_x = x + 2;
    next_y = y - 1;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* right-forward1 */
    next_x = x + 1;
    next_y = y - 2;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* left-backward1 */
    next_x = x - 2;
    next_y = y + 1;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* left-backward2 */
    next_x = x - 1;
    next_y = y + 2;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* right-backward1 */
    next_x = x + 2;
    next_y = y + 1;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
    }

    /* right-backward2 */
    next_x = x + 1;
    next_y = y + 2;
    if (is_valid_xy(next_x, next_y) && get_color(board[next_y][next_x]) != color) {
        insert_front(&head, next_x, next_y);
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
    size_t next_x = x;
    size_t next_y = y + direction;
    if (is_valid_xy(next_x, next_y) && board[next_y][next_x] == 0) {
        insert_front(&head, next_x, next_y);

        /* forward2 */
        size_t next_x2 = next_x;
        size_t next_y2 = next_y + direction;
        if (is_valid_xy(next_x2, next_y2) && board[next_y2][next_x2] == 0) {
            insert_front(&head, next_x2, next_y2);
        }
    }

    /* left-forward */
    next_x = x - 1;
    next_y = y + direction;
    if (is_valid_xy(next_x, next_y) && board[next_y][next_x] != 0) {
        insert_front(&head, next_x, next_y);
    }

    /* right-forward */
    next_x = x + 1;
    next_y = y + direction;
    if (is_valid_xy(next_x, next_y) && board[next_y][next_x] != 0) {
        insert_front(&head, next_x, next_y);
    }

    return head;
}

static void add_moveable_coord_recursion(const piece_t board[][BOARD_WIDTH], const color_t color,
    const size_t x, const size_t y, const int dx, const int dy, node_t** phead)
{
    assert(board != NULL);
    assert(phead != NULL);

    if (!is_valid_xy(x, y)) {
        return;
    }

    piece_t target_piece = board[y][x];
    color_t target_color = get_color(target_piece);

    if (target_color == 0) {
        insert_front(phead, x, y);
        add_moveable_coord_recursion(board, color, x + dx, y + dy, dx, dy, phead);
    }
    else if (target_color != color) {
        insert_front(phead, x, y);
    }
}

static int is_king_moveable(const piece_t board[][BOARD_WIDTH], const size_t src_x, const size_t src_y, const int dx, const int dy)
{
    assert(board != NULL);
    assert(is_valid_xy(src_x, src_y));

    piece_t copied_board[BOARD_HEIGHT][BOARD_WIDTH];
    memcpy(copied_board, board, BOARD_WIDTH * BOARD_HEIGHT * sizeof(piece_t));

    piece_t src_piece = copied_board[src_y][src_x];
    shape_t src_shape = get_shape(src_piece);
    color_t src_color = get_color(src_piece);

    size_t dest_x = src_x + dx;
    size_t dest_y = src_y + dy;
    piece_t dest_piece = copied_board[dest_y][dest_x];
    if (!is_valid_xy(dest_x, dest_y) || get_color(dest_piece) == src_color) {
        return FALSE;
    }

    copied_board[dest_y][dest_x] = src_piece;
    copied_board[src_y][src_x] = 0;

    char coord[COORD_LENGTH];
    for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
        for (size_t x = 0; x < BOARD_WIDTH; ++x) {
            piece_t piece = copied_board[y][x];
            shape_t shape = get_shape(piece);
            color_t color = get_color(piece);

            if (color != src_color) {
                if (shape == SHAPE_KING) {
                    if (is_valid_xy(x - 1, y - 1) && get_shape(copied_board[y - 1][x - 1]) == SHAPE_KING
                        || is_valid_xy(x, y - 1) && get_shape(copied_board[y - 1][x]) == SHAPE_KING
                        || is_valid_xy(x + 1, y - 1) && get_shape(copied_board[y - 1][x + 1]) == SHAPE_KING
                        || is_valid_xy(x - 1, y) && get_shape(copied_board[y][x - 1]) == SHAPE_KING
                        || is_valid_xy(x + 1, y) && get_shape(copied_board[y][x + 1]) == SHAPE_KING
                        || is_valid_xy(x - 1, y + 1) && get_shape(copied_board[y + 1][x - 1]) == SHAPE_KING
                        || is_valid_xy(x, y + 1) && get_shape(copied_board[y + 1][x]) == SHAPE_KING
                        || is_valid_xy(x + 1, y + 1) && get_shape(copied_board[y + 1][x + 1]) == SHAPE_KING) {
                        return FALSE;
                    }
                    continue;
                }
                
                translate_to_coord(x, y, coord);
                node_t* moveable_list = get_moveable_list_or_null(copied_board, coord);
                node_t* p = moveable_list;
                while (p != NULL) {
                    if (p->x == dest_x && p->y == dest_y) {
                        destroy_node(moveable_list);
                        return FALSE;
                    }
                    p = p->next;
                }
                destroy_node(moveable_list);
            }
        }
    }

    return TRUE;
}