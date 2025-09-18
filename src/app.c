// #include <unistd.h>
#include "app.h"
#include "state.h"
#include "renderer.h"
#include "input.h"
#include "lanes.h"
#include "custom_unistd.h"

#include <time.h>
static void sleep_ms(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

static void reset_frog(GameState *gs) {
    gs->frog.x = WIDTH / 2;
    gs->frog.y = HEIGHT - 2;
}

void game_init(GameState *gs, Difficulty diff) {
    gs->running     = 1;
    gs->frog.lives  = LIVES_START;
    gs->frog.score  = 0;
    gs->difficulty  = diff;
    reset_frog(gs);
    lanes_init(gs);
}

static void handle_key(GameState *gs, Key k) {
    int nx = gs->frog.x, ny = gs->frog.y;
    if (k == KEY_UP)         ny--;
    else if (k == KEY_DOWN)  ny++;
    else if (k == KEY_LEFT)  nx--;
    else if (k == KEY_RIGHT) nx++;
    else if (k == KEY_QUIT)  { gs->running = 0; return; }

    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
        gs->frog.x = nx; gs->frog.y = ny;
    }
}

void on_hit(GameState *gs) {
    if (gs->frog.lives > 0) gs->frog.lives--;
    reset_frog(gs);
    if (gs->frog.lives == 0) gs->running = 0;
}

void on_goal(GameState *gs) {
    gs->frog.score += 100;
    reset_frog(gs);
}

// Per-difficulty tick (ms)
static int tick_for(Difficulty d) {
    switch (d) {
        case DIFF_EASY:   return 150;
        case DIFF_MEDIUM: return 90;
        case DIFF_HARD:   return 45;
        default:          return 90;
    }
}

// Difficulty screen: Up/Down to move; Right (or any non-Up/Down/‘q’) confirms; 'q' quits to exit path.
static Difficulty select_difficulty(void) {
    int selected = 1; // default to Medium
    renderer_draw_difficulty_screen(selected);

    for (;;) {
        Key k = input_read_key();
        if (k == KEY_NONE) { sleep_ms(10); continue; }

        if (k == KEY_UP) {
            selected = (selected + 3 - 1) % 3;
            renderer_draw_difficulty_screen(selected);
        } else if (k == KEY_DOWN) {
            selected = (selected + 1) % 3;
            renderer_draw_difficulty_screen(selected);
        } else if (k == KEY_RIGHT) {
            return (Difficulty)selected;
        } else if (k == KEY_QUIT) {
            return QUIT;
        } else {
            // Any other key confirms
            return (Difficulty)selected;
        }
    }
}

static void quit_terminal(void) {
    const char *final = "Thanks for playing! Goodbye :)\n";
    my_syscall(SYS_WRITE, 1, final, 31);
    input_restore();
}

void app_run(void) {
    renderer_draw_start_screen();

    if (input_init_raw() != 0) {
        const char *fail = "Failed to enter raw mode.\n";
        my_syscall(SYS_WRITE, 1, fail, 25);
        return;
    }

    // Wait for any key to leave the start screen (or 'q' to quit)
    for (;;) {
        Key k = input_read_key();
        if (k == KEY_QUIT) { quit_terminal(); return; }
        if (k != KEY_NONE) break;
        sleep_ms(10);
    }

    // Main loop: select difficulty -> run game -> game over -> play again?
    for (;;) {
        // 1) Difficulty selection
        Difficulty diff = select_difficulty();
        if (diff == QUIT) { quit_terminal(); return; }

        // 2) Initialize a fresh game with the chosen difficulty
        GameState gs;
        game_init(&gs, diff);

        const int tick_ms = tick_for(gs.difficulty);

        // 3) Run the game loop
        while (gs.running) {
            Key k = input_read_key();
            if (k != KEY_NONE) handle_key(&gs, k);

            lanes_step(&gs);

            if (gs.frog.y == 0) {
                on_goal(&gs);
            } else if (gs.frog.y != HEIGHT - 1 && lanes_car_at(&gs, gs.frog.x, gs.frog.y)) {
                on_hit(&gs);
            }

            renderer_draw_game(&gs);
            sleep_ms(tick_ms);
        }

        // 4) Game over and replay prompt
        renderer_draw_game_over(&gs);
        // For 2 seconds, clear any buffered input and just wait
        for (int a = 0; a < 20; a++) {
            unsigned char drain_buf[8];
            while ((my_syscall(SYS_READ, STDIN_FILENO, drain_buf, sizeof(drain_buf))) > 0) {}
            sleep_ms(100);
        }
        for (;;) {
            Key k = input_read_key();
            if (k == KEY_NONE) { sleep_ms(10); continue; }
            if (k == KEY_QUIT) { quit_terminal(); return; } // 'q' exits entirely
            break; // any other key -> play again (back to difficulty selection)
        }
    }
    quit_terminal();
}

// _start denotes program entry point when using -nostartfiles
int _start(void) {
    app_run();
    my_exit(0);
    return 0;
}
