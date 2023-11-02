#ifndef GAME_H
#define GAME_H 1
#include "common.h"
#include "game_code.h"
#include "game_state.h"

EXPORT_FUNC game_code_t* game_get_code(void);
EXPORT_FUNC void game_init(game_state_t *state);
EXPORT_FUNC void game_tick(void);
EXPORT_FUNC void game_shutdown(void);
EXPORT_FUNC void game_load_code(game_state_t *state);
EXPORT_FUNC void game_unload_code(void);
EXPORT_FUNC int game_should_continue(void);

#endif // GAME_H
