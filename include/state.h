#ifndef STATE_H
#define STATE_H

#define WIDTH      41
#define HEIGHT     21
#define MAX_LANES  8
#define LIVES_START 3
#define TICK_MS    200

typedef struct {
    int y;        /* row in playfield */
    int dir;      /* -1 left, +1 right */
    int speed;    /* ticks per move */
    int tick;     /* internal counter */
    int gap;      /* spaces between cars */
    int car_len;  /* car length */
    int offset;   /* phase within period */
    char car_ch;  /* '<' or '>' */
} Lane;

typedef struct {
    int x, y;
    int lives;
    int score;
} Frog;

typedef enum {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
} Difficulty;

typedef struct {
    Frog frog;
    Lane lanes[MAX_LANES];
    int lane_count;
    int running;
    Difficulty difficulty;
} GameState;

void game_init(GameState *gs, Difficulty diff);
void on_goal(GameState *gs);
void on_hit(GameState *gs);

#endif
