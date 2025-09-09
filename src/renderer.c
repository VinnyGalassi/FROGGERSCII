#include <unistd.h>
#include "renderer.h"
#include "state.h"
#include "lanes.h"

static void clear_and_home(void) {
    const char *esc = "\x1b[2J\x1b[H";
    for (const char *p = esc; *p; ++p) write(1, p, 1);
}

static void draw_border_line(void) {
    char buf[WIDTH + 3];
    int i = 0;
    buf[i++] = '+';
    for (int x = 0; x < WIDTH; x++) buf[i++] = '-';
    buf[i++] = '+';
    buf[i++] = '\n';
    write(1, buf, i);
}

static void draw_grid(const GameState *gs) {
    draw_border_line();
    char line[WIDTH + 3];
    for (int y = 0; y < HEIGHT; y++) {
        int i = 0;
        line[i++] = '|';
        for (int x = 0; x < WIDTH; x++) {
            char ch = ' ';
            if (y == 0) ch = '=';
            else if (y == HEIGHT - 1) ch = '#';
            else {
                char lc = lanes_char_at(gs, x, y);
                if (lc) ch = lc;
            }
            if (x == gs->frog.x && y == gs->frog.y) ch = 'F';
            line[i++] = ch;
        }
        line[i++] = '|';
        line[i++] = '\n';
        write(1, line, i);
    }
    draw_border_line();
}

void renderer_draw_start_screen(void) {
    clear_and_home();
    const char *msg =
        "+------------------------------------------+\n"
        "|               ASCII FROGGER              |\n"
        "+------------------------------------------+\n"
        "\n"
        "  Press any key to start\n"
        "  Press 'q' to quit\n"
        "\n"
        "  > ";
    for (const char *p = msg; *p; ++p) write(1, p, 1);
}

// Helper to write an integer to stdout
static void write_int(int n) {
    char buf[12];
    int i = 11;
    buf[i--] = '\0';
    int neg = n < 0;
    unsigned int u = neg ? -n : n;
    do {
        buf[i--] = '0' + (u % 10);
        u /= 10;
    } while (u);
    if (neg) buf[i--] = '-';
    write(1, buf + i + 1, 11 - i);
}

void renderer_draw_game(const GameState *gs) {
    clear_and_home();
    draw_grid(gs);
    const char *lives = "Lives: ";
    write(1, lives, 7);
    write_int(gs->frog.lives);
    const char *score = "   Score: ";
    write(1, score, 10);
    write_int(gs->frog.score);
    const char *rest = "   (Arrows/WASD, q quits)\n";
    write(1, rest, 27);
}

void renderer_draw_game_over(const GameState *gs) {
    clear_and_home();
    const char *msg =
        "+------------------+\n"
        "|     GAME OVER    |\n"
        "+------------------+\n";
    for (const char *p = msg; *p; ++p) write(1, p, 1);
    const char *fs = "Final Score: ";
    write(1, fs, 13);
    write_int(gs->frog.score);
    const char *end = "\n\nPress Enter to exit.\n";
    write(1, end, 24);
}
