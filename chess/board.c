#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "input.h"
#include "piece.h"
#include "validations.h"

static piece_t s_board[BOARD_HEIGHT][BOARD_WIDTH];
static color_t s_cur_turn;

void init_board(void)
{
    const size_t WHITE_KING_Y = 7;
    const size_t BLACK_KING_Y = 0;

    const size_t WHITE_MAJOR_Y = 7;
    const size_t BLACK_MAJOR_Y = 0;

    const size_t WHITE_MINOR_Y = 7;
    const size_t BLACK_MINOR_Y = 0;

    const size_t WHITE_PAWN_Y = WHITE_KING_Y - 1;
    const size_t BLACK_PAWN_Y = BLACK_KING_Y + 1;

    const size_t KING_X = 4;
    const size_t QUEEN_X = 3;

    const size_t LEFT_ROOK_X = 0;
    const size_t RIGHT_ROOK_X = 7;

    const size_t LEFT_BISHOP_X = 2;
    const size_t RIGHT_BISHOP_X = 5;

    const size_t LEFT_KNIGHT_X = 1;
    const size_t RIGHT_KNIGHT_X = 6;

    // king
    s_board[WHITE_KING_Y][KING_X] = SHAPE_KING | COLOR_WHITE;
    s_board[BLACK_KING_Y][KING_X] = SHAPE_KING | COLOR_BLACK;

    // queen
    s_board[WHITE_MAJOR_Y][QUEEN_X] = SHAPE_QUEEN | COLOR_WHITE;
    s_board[BLACK_MAJOR_Y][QUEEN_X] = SHAPE_QUEEN | COLOR_BLACK;

    // rook
    s_board[WHITE_MAJOR_Y][LEFT_ROOK_X] = SHAPE_ROOK | COLOR_WHITE;
    s_board[WHITE_MAJOR_Y][RIGHT_ROOK_X] = SHAPE_ROOK | COLOR_WHITE;
    s_board[BLACK_MAJOR_Y][LEFT_ROOK_X] = SHAPE_ROOK | COLOR_BLACK;
    s_board[BLACK_MAJOR_Y][RIGHT_ROOK_X] = SHAPE_ROOK | COLOR_BLACK;

    // bishop
    s_board[WHITE_MINOR_Y][LEFT_BISHOP_X] = SHAPE_BISHOP | COLOR_WHITE;
    s_board[WHITE_MINOR_Y][RIGHT_BISHOP_X] = SHAPE_BISHOP | COLOR_WHITE;
    s_board[BLACK_MINOR_Y][LEFT_BISHOP_X] = SHAPE_BISHOP | COLOR_BLACK;
    s_board[BLACK_MINOR_Y][RIGHT_BISHOP_X] = SHAPE_BISHOP | COLOR_BLACK;

    // knight
    s_board[WHITE_MINOR_Y][LEFT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_WHITE;
    s_board[WHITE_MINOR_Y][RIGHT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_WHITE;
    s_board[BLACK_MINOR_Y][LEFT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_BLACK;
    s_board[BLACK_MINOR_Y][RIGHT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_BLACK;

    // pawn
    for (size_t i = 0; i < BOARD_WIDTH; ++i) {
        s_board[WHITE_PAWN_Y][i] = SHAPE_PAWN | COLOR_WHITE;
        s_board[BLACK_PAWN_Y][i] = SHAPE_PAWN | COLOR_BLACK;
    }

    s_cur_turn = COLOR_WHITE;
}

static void move(piece_t board[][BOARD_WIDTH], const size_t src_x, const size_t src_y, const size_t dest_x, const size_t dest_y);

void update_board(void)
{
    assert(is_valid_coord(g_src_coord));
    assert(is_valid_coord(g_dest_coord));

    size_t src_x = translate_to_board_x(g_src_coord);
    size_t src_y = translate_to_board_y(g_src_coord);
    size_t dest_x = translate_to_board_x(g_dest_coord);
    size_t dest_y = translate_to_board_y(g_dest_coord);

    piece_t selected_piece = s_board[src_y][src_x];
    if (get_color(selected_piece) != s_cur_turn) {
        printf("it's not your turn\n");
        return;
    }

    node_t* movable_list = get_movable_list_or_null(s_board, g_src_coord);
    print_list(movable_list);

    node_t* p = movable_list;
    while (p != NULL) {
        if (p->x == dest_x && p->y == dest_y) {
            move(s_board, src_x, src_y, dest_x, dest_y);
            break;
        }

        p = p->next;
    }

    if (p == NULL) {
        printf("illegal moves\n\n");
    }

    destroy_list(movable_list);

    s_cur_turn = (s_cur_turn == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
}

void draw_board(void)
{
    const char* VERTICAL_BOUNDARY = "-----------------------------------------";
    const char* HORIZONTAL_BOUNDARY = "|";

    for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
        printf(" %s\n", VERTICAL_BOUNDARY);

        for (size_t x = 0; x < BOARD_WIDTH; ++x) {
            piece_t piece = s_board[y][x];
            shape_t shape = get_shape(piece);
            color_t color = get_color(piece);
            char display_name[3];

            switch (color) {
            case COLOR_BLACK:
                display_name[0] = 'B';
                break;
            case COLOR_WHITE:
                display_name[0] = 'W';
                break;
            default:
                break;
            }

            switch (shape) {
            case SHAPE_KING:
                display_name[1] = 'K';
                break;
            case SHAPE_QUEEN:
                display_name[1] = 'Q';
                break;
            case SHAPE_ROOK:
                display_name[1] = 'R';
                break;
            case SHAPE_BISHOP:
                display_name[1] = 'B';
                break;
            case SHAPE_KNIGHT:
                display_name[1] = 'N';
                break;
            case SHAPE_PAWN:
                display_name[1] = 'P';
                break;
            default:
                display_name[0] = ' ';
                display_name[1] = ' ';
                break;
            }

            display_name[2] = '\0';

            printf("%2s%3s", HORIZONTAL_BOUNDARY, display_name);
        }
        printf("%2s\n", HORIZONTAL_BOUNDARY);
    }
    printf(" %s\n", VERTICAL_BOUNDARY);
}

int is_checkmate(void) {
    char coord[COORD_LENGTH];

    for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
        for (size_t x = 0; x < BOARD_WIDTH; ++x) {
            color_t color = get_color(s_board[y][x]);
            if (color == s_cur_turn) {
                translate_to_coord(x, y, coord);
                node_t* movable_list = get_movable_list_or_null(s_board, coord);
                if (movable_list != NULL) {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

size_t translate_to_board_x(const char* coord)
{
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    return coord[0] - 'a';
}

size_t translate_to_board_y(const char* coord)
{
    assert(coord != NULL);
    assert(is_valid_coord(coord));

    return 7 - (coord[1] - '1');
}

void translate_to_coord(const size_t x, const size_t y, char* out_coord)
{
    assert(is_valid_x(x));
    assert(is_valid_y(y));

    out_coord[0] = (char)x + 'a';
    out_coord[1] = '8' - (char)y;
    out_coord[2] = '\0';
    assert(is_valid_coord(out_coord));
}

static void move(piece_t board[][BOARD_WIDTH], const size_t src_x, const size_t src_y, const size_t dest_x, const size_t dest_y)
{
    assert(board != NULL);
    assert(is_valid_xy(src_x, src_y));
    assert(is_valid_xy(dest_x, dest_y));

    board[dest_y][dest_x] = board[src_y][src_x];
    board[src_y][src_x] = 0;

    board[dest_y][dest_x] |= MOVE_FLAG;
}