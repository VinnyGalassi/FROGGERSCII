#include <stdio.h>
#include "renderer.h"
#include "state.h"
#include "lanes.h"

static void clear_and_home(void) { fputs("\x1b[2J\x1b[H", stdout); }

static void draw_border_line(void) {
    putchar('+');
    for (int x = 0; x < WIDTH; x++) putchar('-');
    puts("+");
}

static void draw_grid(const GameState *gs) {
    draw_border_line();

    for (int y = 0; y < HEIGHT; y++) {
        putchar('|');
        for (int x = 0; x < WIDTH; x++) {
            char ch = ' ';
            if (y == 0) ch = '=';
            else if (y == HEIGHT - 1) ch = '#';
            else {
                char lc = lanes_char_at(gs, x, y);
                if (lc) ch = lc;
            }
            if (x == gs->frog.x && y == gs->frog.y) ch = 'F';
            putchar(ch);
        }
        puts("|");
    }

    draw_border_line();
}

void renderer_draw_start_screen(void) {
    clear_and_home();
    puts("+------------------------------------------+");
    puts("|               ASCII FROGGER              |");
    puts("+------------------------------------------+");
    puts("");
    puts("  Press any key to start");
    puts("  Press 'q' to quit");
    puts("");
    fputs("  > ", stdout);
    fflush(stdout);
}

void renderer_draw_game(const GameState *gs) {
    clear_and_home();
    draw_grid(gs);
    printf("Lives: %d   Score: %d   (Arrows/WASD, q quits)\n", gs->frog.lives, gs->frog.score);
    fflush(stdout);
}

void renderer_draw_game_over(const GameState *gs) {
    clear_and_home();
    puts("+------------------+");
    puts("|     GAME OVER    |");
    puts("+------------------+");
    printf("Final Score: %d\n", gs->frog.score);
    puts("\nPress Enter to exit.");
}
