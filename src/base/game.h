#ifndef GAME_H
#define GAME_H 1
#include "game_state.h"

void game_load_code(void);
void game_init(void);
void game_tick(float delta_time);
int game_should_continue(void);
void game_shutdown(void);

#endif // GAME_H
