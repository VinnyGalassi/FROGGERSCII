// #include <unistd.h>
#include "app.h"
#include "state.h"
#include "renderer.h"
#include "input.h"
#include "lanes.h"
#include "custom_unistd.h"

#include <time.h>
static void sleep_ms(int ms) {
    // if (ms > 0) usleep(ms * 1000);
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

static void reset_frog(GameState *gs) {
    gs->frog.x = WIDTH / 2;
    gs->frog.y = HEIGHT - 2;
}

static void game_init(GameState *gs) {
    gs->running   = 1;
    gs->frog.lives = LIVES_START;
    gs->frog.score = 0;
    reset_frog(gs);
    lanes_init(gs);
}

static void handle_key(GameState *gs, Key k) {
    int nx = gs->frog.x, ny = gs->frog.y;
    if (k == KEY_UP)    ny--;
    else if (k == KEY_DOWN) ny++;
    else if (k == KEY_LEFT) nx--;
    else if (k == KEY_RIGHT) nx++;
    else if (k == KEY_QUIT)  { gs->running = 0; return; }

    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
        gs->frog.x = nx; gs->frog.y = ny;
    }
}

static void on_hit(GameState *gs) {
    if (gs->frog.lives > 0) gs->frog.lives--;
    reset_frog(gs);
    if (gs->frog.lives == 0) gs->running = 0;
}

static void on_goal(GameState *gs) {
    gs->frog.score += 100;
    reset_frog(gs);
}

void app_run(void) {
    GameState gs;
    game_init(&gs);

    renderer_draw_start_screen();

    if (input_init_raw() != 0) {
        const char *fail = "Failed to enter raw mode.\n";
        my_write(1, fail, 25);
        return;
    }

    for (;;) {
        Key k = input_read_key();
        if (k == KEY_QUIT) { input_restore(); return; }
        if (k != KEY_NONE) break;
        sleep_ms(10);
    }

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
        sleep_ms(TICK_MS);
    }

    input_restore();
    renderer_draw_game_over(&gs);
}
