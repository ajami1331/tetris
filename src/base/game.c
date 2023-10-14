#include "game.h"
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
HMODULE handle;
#endif

typedef void (*game_init_t)(void);
typedef void (*game_tick_t)(float delta_time);
typedef void (*game_terminate_t)(void);
typedef int (*game_should_continue_t)(void);
typedef void (*game_load_code_t)(void);
typedef void (*game_unload_code_t)(void);

game_init_t game_init_dynamic;
game_tick_t game_tick_dynamic;
game_terminate_t game_terminate_dynamic;
game_should_continue_t game_should_continue_dynamic;
game_load_code_t game_load_code_dynamic;
game_unload_code_t game_unload_code_dynamic;
int last_dll_write_time = 0;

void unload_game_code(void);

void game_load_code(void)
{
#ifdef DEBUG_MODE
#ifdef _WIN32
    CopyFile("game.dll", "game1.dll", 0);
    handle = LoadLibraryA("game1.dll");
    last_dll_write_time = GetFileModTime("fl");
    game_init_dynamic = (game_init_t) GetProcAddress(handle, "game_init");
    game_tick_dynamic = (game_tick_t) GetProcAddress(handle, "game_tick");
    game_terminate_dynamic = (game_terminate_t) GetProcAddress(handle, "game_terminate");
    game_should_continue_dynamic = (game_should_continue_t) GetProcAddress(handle, "game_should_continue");
    game_load_code_dynamic = (game_load_code_t) GetProcAddress(handle, "game_load_code");
    game_unload_code_dynamic = (game_unload_code_t) GetProcAddress(handle, "game_unload_code");
#endif // _WIN32
#endif // DEBUG_MODE
    if (game_load_code_dynamic)
    {
        game_load_code_dynamic();
    }
}

void game_init(void)
{
    if (game_init_dynamic)
    {
        game_init_dynamic();
    }
}

void game_tick(float delta_time)
{
    if (game_tick_dynamic)
    {
        game_tick_dynamic(delta_time);
    }
#ifdef DEBUG_MODE
#ifdef _WIN32
    if (GetFileModTime("fl") != last_dll_write_time)
    {
        unload_game_code();
        game_load_code();
    }
#endif // _WIN32
#endif // DEBUG_MODE
}

void game_terminate(void)
{
    if (game_terminate_dynamic)
    {
        game_terminate_dynamic();
    }
#ifdef DEBUG_MODE
#ifdef _WIN32
    if (handle)
    {
        FreeLibrary(handle);
    }
#endif // _WIN32
#endif // DEBUG_MODE
}

int game_should_continue(void)
{
    if (game_should_continue_dynamic)
    {
        return game_should_continue_dynamic();
    }

    return 0;
}

void unload_game_code(void)
{
#ifdef _WIN32
    if (game_unload_code_dynamic)
    {
        game_unload_code_dynamic();
    }
    if (handle)
    {
        FreeLibrary(handle);
        game_init_dynamic = NULL;
        game_tick_dynamic = NULL;
        game_terminate_dynamic = NULL;
        game_should_continue_dynamic = NULL;
        game_load_code_dynamic = NULL;
        game_unload_code_dynamic = NULL;
    }
#endif // _WIN32
}