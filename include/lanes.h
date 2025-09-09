#ifndef LANES_H
#define LANES_H
#include "state.h"

void lanes_init(GameState *gs);
void lanes_step(GameState *gs);
int  lanes_car_at(const GameState *gs, int x, int y);
char lanes_char_at(const GameState *gs, int x, int y);

#endif
