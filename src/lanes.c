#include <stdint.h>
#include "lanes.h"
#include "state.h"

static uint32_t xs = 0x9E3779B1u;
static inline uint32_t xorshift32(void){
    uint32_t x = xs;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    return xs = x;
}
static inline unsigned randi(unsigned n){ return (unsigned)(xorshift32() % n); }

static int lane_has_car(const Lane* L, int x){
    int P = L->gap + L->car_len;
    int u = (x + L->offset) % P;
    return (u < L->car_len);
}

void lanes_init(GameState *gs) {
    gs->lane_count = 0;
    int y = 2;

    gs->lanes[gs->lane_count++] = (Lane){ .y=y++, .dir=+1, .speed=3, .tick=0, .gap=6, .car_len=3, .offset=(int)randi(9),  .car_ch='>' };
    gs->lanes[gs->lane_count++] = (Lane){ .y=y++, .dir=-1, .speed=2, .tick=0, .gap=7, .car_len=2, .offset=(int)randi(9),  .car_ch='<' };
    gs->lanes[gs->lane_count++] = (Lane){ .y=y++, .dir=+1, .speed=2, .tick=0, .gap=5, .car_len=2, .offset=(int)randi(7),  .car_ch='>' };
    gs->lanes[gs->lane_count++] = (Lane){ .y=y++, .dir=-1, .speed=1, .tick=0, .gap=6, .car_len=3, .offset=(int)randi(9),  .car_ch='<' };
    gs->lanes[gs->lane_count++] = (Lane){ .y=y++, .dir=+1, .speed=3, .tick=0, .gap=8, .car_len=2, .offset=(int)randi(10), .car_ch='>' };
    gs->lanes[gs->lane_count++] = (Lane){ .y=y++, .dir=-1, .speed=2, .tick=0, .gap=6, .car_len=3, .offset=(int)randi(9),  .car_ch='<' };
}

void lanes_step(GameState *gs) {
    float scale = 1.0f;
    switch (gs->difficulty) {
        case DIFF_EASY:   scale = 1.6f; break;  // noticeably slower
        case DIFF_MEDIUM: scale = 1.2f; break;  // baseline
        case DIFF_HARD:   scale = 0.8f; break;  // noticeably faster
        default:          scale = 1.0f; break;
    }

    for (int i = 0; i < gs->lane_count; i++) {
        Lane *L = &gs->lanes[i];
        if (++L->tick >= L->speed) {
            L->tick = 0;
            int P = L->gap + L->car_len;
            if (L->dir > 0) L->offset = (L->offset + 1) % P;
            else            L->offset = (L->offset + P - 1) % P;
        }
    }
}

int lanes_car_at(const GameState *gs, int x, int y) {
    for (int i = 0; i < gs->lane_count; i++) {
        const Lane *L = &gs->lanes[i];
        if (L->y == y) {
            if (lane_has_car(L, x)) return 1;
        }
    }
    return 0;
}

char lanes_char_at(const GameState *gs, int x, int y) {
    for (int i = 0; i < gs->lane_count; i++) {
        const Lane *L = &gs->lanes[i];
        if (L->y == y && lane_has_car(L, x)) return L->car_ch;
    }
    return 0;
}
