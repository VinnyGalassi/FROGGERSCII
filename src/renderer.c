#include "renderer.h"
#include "state.h"
#include "lanes.h"
#include "custom_unistd.h"

static void clear_and_home(void) {
    const char *esc = "\x1b[2J\x1b[H";
    my_syscall(SYS_WRITE, 1, esc, 7);
}

static void draw_border_line(void) {
    char buf[WIDTH + 3];
    int i = 0;
    buf[i++] = '+';
    for (int x = 0; x < WIDTH; x++) buf[i++] = '-';
    buf[i++] = '+';
    buf[i++] = '\n';
    my_syscall(SYS_WRITE, 1, buf, i);
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
        my_syscall(SYS_WRITE, 1, line, i);
    }
    draw_border_line();
}

void renderer_draw_start_screen(void) {
    clear_and_home();
    const char *msg =
        "\n+------------------------------------------+\n"
        "|               ASCII FROGGER              |\n"
        "+------------------------------------------+\n"
        "\n"
        "  Press arrow keys to start\n"
        "  Press 'q' to quit\n"
        "\n"
        "  > ";
    my_syscall(SYS_WRITE, 1, msg, 190);
}

// Helper to my_syscall SYS_WRITE, an integer to stdout
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
    my_syscall(SYS_WRITE, 1, buf + i + 1, 11 - i);
}

void renderer_draw_game(const GameState *gs) {
    clear_and_home();
    draw_grid(gs);
    const char *lives = "Lives: ";
    my_syscall(SYS_WRITE, 1, lives, 7);
    write_int(gs->frog.lives);
    const char *score = "   Score: ";
    my_syscall(SYS_WRITE, 1, score, 10);
    write_int(gs->frog.score);
    const char *rest = "   (Arrows/WASD, q quits)\n";
    my_syscall(SYS_WRITE, 1, rest, 27);
}

void renderer_draw_game_over(const GameState *gs) {
    clear_and_home();
    const char *msg =
        "\n+------------------+\n"
        "|     GAME OVER    |\n"
        "+------------------+\n";
    my_syscall(SYS_WRITE, 1, msg, 64);
    const char *fs = "Final Score: ";
    my_syscall(SYS_WRITE, 1, fs, 13);
    write_int(gs->frog.score);
    const char *end = "\n\nPress arrow keys to play again!\n\nPress [Q] to exit :(\n";
    my_syscall(SYS_WRITE, 1, end, 57);
}

void renderer_draw_difficulty_screen(int selected_idx) {
    clear_and_home();

    const char *title = "\nSelect Difficulty\n\n";
    my_syscall(SYS_WRITE, 1, title, 19);

    const char *items[3] = { "Easy  \n", "Medium\n", "Hard  \n" };
    for (int i = 0; i < 3; i++) {
        if (i == selected_idx) {
            const char *arrow = "> ";
            my_syscall(SYS_WRITE, 1, arrow, 2);
        } else {
            const char *space = "  ";
            my_syscall(SYS_WRITE, 1, space, 2);
        }
        const char *label = items[i];
        my_syscall(SYS_WRITE, 1, label, 7);
    }

    const char *help = "\n[Up/Down] move  [Right/Left] confirm  [Q] quit\n";
    my_syscall(SYS_WRITE, 1, help, 48);
}
