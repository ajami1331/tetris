#ifndef GAME_H
#define GAME_H 1
#include "common.h"
#include "game_code.h"

EXPORT_FUNC game_code_t* game_get_code(void);
EXPORT_FUNC void game_init(void);
EXPORT_FUNC void game_tick();
EXPORT_FUNC void game_shutdown(void);
EXPORT_FUNC void game_load_code(void);
EXPORT_FUNC void game_unload_code(void);
EXPORT_FUNC int game_should_continue(void);

#endif // GAME_H
