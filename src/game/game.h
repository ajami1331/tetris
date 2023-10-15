#ifndef GAME_H
#define GAME_H 1
#include "common.h"

EXPORT_FUNC void game_init(void);
EXPORT_FUNC void game_tick(float delta_time);
EXPORT_FUNC void game_terminate(void);
EXPORT_FUNC void game_load_code(void);
EXPORT_FUNC void game_unload_code(void);
EXPORT_FUNC int game_should_continue(void);

#endif // GAME_H
