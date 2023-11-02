#ifndef GAME_STATE_H
#define GAME_STATE_H 1

#include "tetrimino.h"

typedef struct
{
    int block_size;
    tetrimino_t current_tetrimino;
} game_state_t;

#endif // GAME_STATE_H
