#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "input.h"
#include "piece.h"
#include "validations.h"

static piece_t s_board[BOARD_HEIGHT][BOARD_WIDTH];
static color_t s_cur_turn;

static void update_board_mock(void);

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

    /* king */
    s_board[WHITE_KING_Y][KING_X] = SHAPE_KING | COLOR_WHITE;
    s_board[BLACK_KING_Y][KING_X] = SHAPE_KING | COLOR_BLACK;

    /* queen */
    s_board[WHITE_MAJOR_Y][QUEEN_X] = SHAPE_QUEEN | COLOR_WHITE;
    s_board[BLACK_MAJOR_Y][QUEEN_X] = SHAPE_QUEEN | COLOR_BLACK;

    /* rook */
    s_board[WHITE_MAJOR_Y][LEFT_ROOK_X] = SHAPE_ROOK | COLOR_WHITE;
    s_board[WHITE_MAJOR_Y][RIGHT_ROOK_X] = SHAPE_ROOK | COLOR_WHITE;
    s_board[BLACK_MAJOR_Y][LEFT_ROOK_X] = SHAPE_ROOK | COLOR_BLACK;
    s_board[BLACK_MAJOR_Y][RIGHT_ROOK_X] = SHAPE_ROOK | COLOR_BLACK;

    /* bishop */
    s_board[WHITE_MINOR_Y][LEFT_BISHOP_X] = SHAPE_BISHOP | COLOR_WHITE;
    s_board[WHITE_MINOR_Y][RIGHT_BISHOP_X] = SHAPE_BISHOP | COLOR_WHITE;
    s_board[BLACK_MINOR_Y][LEFT_BISHOP_X] = SHAPE_BISHOP | COLOR_BLACK;
    s_board[BLACK_MINOR_Y][RIGHT_BISHOP_X] = SHAPE_BISHOP | COLOR_BLACK;

    /* knight */
    s_board[WHITE_MINOR_Y][LEFT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_WHITE;
    s_board[WHITE_MINOR_Y][RIGHT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_WHITE;
    s_board[BLACK_MINOR_Y][LEFT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_BLACK;
    s_board[BLACK_MINOR_Y][RIGHT_KNIGHT_X] = SHAPE_KNIGHT | COLOR_BLACK;

    /* pawn */
    for (size_t i = 0; i < BOARD_WIDTH; ++i) {
        s_board[WHITE_PAWN_Y][i] = SHAPE_PAWN | COLOR_WHITE;
        s_board[BLACK_PAWN_Y][i] = SHAPE_PAWN | COLOR_BLACK;
    }

    s_cur_turn = COLOR_WHITE;
}

void update_board(void)
{
    update_board_mock();
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

static void update_board_mock(void)
{
    size_t from_x = translate_to_board_x(g_from_coord);
    size_t from_y = translate_to_board_y(g_from_coord);
    size_t to_x = translate_to_board_x(g_to_coord);
    size_t to_y = translate_to_board_y(g_to_coord);

    node_t* moveable_list = NULL;
    piece_t selected_piece = s_board[from_y][from_x];

    printf("\n");

    if (selected_piece == 0) {
        printf("please, select piece\n\n");
        goto exit;
    }

    if (get_color(selected_piece) != s_cur_turn) {
        printf("not your turn\n\n");
        goto exit;
    }

    moveable_list = get_moveable_list_or_null(s_board, g_from_coord);
    node_t* p = moveable_list;
    while (p != NULL) {
        if (p->x == to_x && p->y == to_y) {
            s_board[to_y][to_x] = s_board[from_y][from_x];
            s_board[from_y][from_x] = 0;
            break;
        }

        p = p->next;
    }

    printf("moveable coordinates: ");
    print_node(moveable_list);
    printf("\n");

    if (p == NULL) {
        printf("you can't move there\n");
        goto exit;
    }

    s_cur_turn = (s_cur_turn == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;

exit:
    destroy_node(moveable_list);
}