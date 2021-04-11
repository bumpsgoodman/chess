#include <assert.h>

#include "game.h"
#include "board.h"
#include "input.h"

int main(void)
{
    int b_running = TRUE;

    init_game();
    draw_game();
    
    while (b_running) {
        input();
        update_game();
        draw_game();

        b_running = !is_checkmate();
    }

    return 0;
}

void init_game(void)
{
    init_board();
}

void update_game(void)
{
    update_board();
}

void draw_game(void)
{
    draw_board();
}