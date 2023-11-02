#ifndef GAME_H
#define GAME_H 1
#include "game_state.h"

void game_load_code(game_state_t *state);
void game_init(game_state_t *state);
void game_tick(void);
int game_should_continue(void);
void game_shutdown(void);

#endif // GAME_H
