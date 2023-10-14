#include "game.h"
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
HMODULE handle;
#endif // _WIN32

#if __linux__ || __APPLE__
#include <dlfcn.h>
#include <raylib.h>
void *handle;
#endif // __linux__ || __APPLE__

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
#ifdef _WIN32
    CopyFile("game.dll", "game1.dll", 0);
    handle = LoadLibraryA("game1.dll");
    last_dll_write_time = GetFileModTime("fl");
    game_init_dynamic = (game_init_t)GetProcAddress(handle, "game_init");
    game_tick_dynamic = (game_tick_t)GetProcAddress(handle, "game_tick");
    game_terminate_dynamic = (game_terminate_t)GetProcAddress(handle, "game_terminate");
    game_should_continue_dynamic = (game_should_continue_t)GetProcAddress(handle, "game_should_continue");
    game_load_code_dynamic = (game_load_code_t)GetProcAddress(handle, "game_load_code");
    game_unload_code_dynamic = (game_unload_code_t)GetProcAddress(handle, "game_unload_code");
#endif // _WIN32
#if __linux__ || __APPLE__
    handle = dlopen("./libgame.so", RTLD_LAZY);
    last_dll_write_time = GetFileModTime("./libgame.so");
    *(void **) (&game_init_dynamic) = dlsym(handle, "game_init");
    *(void **) (&game_tick_dynamic) = dlsym(handle, "game_tick");
    *(void **) (&game_terminate_dynamic) = dlsym(handle, "game_terminate");
    *(int **) (&game_should_continue_dynamic) = dlsym(handle, "game_should_continue");
    *(void **) (&game_load_code_dynamic) = dlsym(handle, "game_load_code");
    *(void **) (&game_unload_code_dynamic) = dlsym(handle, "game_unload_code");
#endif // __linux__ || __APPLE__
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
    if (GetFileModTime("fl") > 10 + last_dll_write_time)
    {
        unload_game_code();
        game_load_code();
    }
#endif // _WIN32
#if __linux__ || __APPLE__
    if (GetFileModTime("./libgame.so") != last_dll_write_time)
    {
        unload_game_code();
        game_load_code();
    }
#endif // __linux__ || __APPLE__
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
    if (game_unload_code_dynamic)
    {
        game_unload_code_dynamic();
    }
    
    if (handle)
    {
#ifdef _WIN32
        FreeLibrary(handle);
#endif // _WIN32
#if __linux__ || __APPLE__
        dlclose(handle);
#endif // __linux__ || __APPLE__
        game_init_dynamic = 0;
        game_tick_dynamic = 0;
        game_terminate_dynamic = 0;
        game_should_continue_dynamic = 0;
        game_load_code_dynamic = 0;
        game_unload_code_dynamic = 0;
    }
}