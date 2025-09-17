#ifndef RENDERER_H
#define RENDERER_H
#include "state.h"

void renderer_draw_start_screen(void);
void renderer_draw_game(const GameState *gs);
void renderer_draw_game_over(const GameState *gs);
void renderer_draw_difficulty_screen(int selected_idx);

#endif
