#ifndef TETRIMINO_H
#define TETRIMINO_H 1

typedef enum 
{
    TETRIMINO_I,
    TETRIMINO_O,
    TETRIMINO_T,
    TETRIMINO_S,
    TETRIMINO_Z,
    TETRIMINO_J,
    TETRIMINO_L,
    TETRIMINO_COUNT
} tetrimino_type;

typedef struct 
{
    tetrimino_type type;
    float x;
    float y;
    char blocks[4][4];
} tetrimino_t;

#endif // TETRIMINO_H