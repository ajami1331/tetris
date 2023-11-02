#ifndef GAME_CODE_H
#define GAME_CODE_H 1

#include "game_state.h"

typedef struct
{
    void (*init)(game_state_t *state);
    void (*tick)(void);
    void (*shutdown)(void);
    void (*load_code)(game_state_t *state);
    void (*unload_code)(void);
    int (*should_continue)(void);
} game_code_t;


#endif // GAME_CODE_H
