#ifndef GAME_H
#define GAME_H 1
#include "game_state.h"

void game_load_code(void);
void game_init(void);
void game_tick(float delta_time);
void game_terminate(void);

#endif // GAME_H