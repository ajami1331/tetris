#ifndef GAME_CODE_H
#define GAME_CODE_H 1

typedef struct
{
    void (*init)(void);
    void (*tick)(void);
    void (*shutdown)(void);
    void (*load_code)(void);
    void (*unload_code)(void);
    int (*should_continue)(void);
} game_code_t;


#endif // GAME_CODE_H
