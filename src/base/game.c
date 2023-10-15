#include "game.h"
#include "game_code.h"
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
HMODULE handle;
#endif // _WIN32

#ifdef PLATFORM_WEB
#include "../game/game.h"
#endif // PLATFORM_WEB

#if __linux__ || __APPLE__
#include <dlfcn.h>
#include <raylib.h>
void *handle;
#endif // __linux__ || __APPLE__

typedef game_code_t *(*game_get_code_t)(void);

game_get_code_t game_get_code_dynamic;

void game_unload_code(void);

#if __linux__ || __APPLE__
int last_dll_write_time = 0;
#endif // __linux__ || __APPLE__

#ifdef _WIN32
#ifdef __MINGW32__
#define GAME_CODE_LIB "libgame.dll"
#else
#define GAME_CODE_LIB "game.dll"
#endif // __MINGW32__
#elif __linux__
#define GAME_CODE_LIB "./libgame.so"
#elif __APPLE__
#define GAME_CODE_LIB "./libgame.dylib"
#endif // _WIN32

#ifdef _WIN32

DWORD last_dll_write_time = 0;

DWORD GetFileModTime(const char *path)
{
    FILETIME last_write_time = {0};
    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file != INVALID_HANDLE_VALUE)
    {
        GetFileTime(file, 0, 0, &last_write_time);
        CloseHandle(file);
    }
    return last_write_time.dwLowDateTime;
}
#endif // _WIN32

game_code_t *game_code = 0;

void game_load_code(void)
{
#ifdef _WIN32
#ifdef __MINGW32__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
    CopyFileA(GAME_CODE_LIB, "game1.dll", 0);
#else
    CopyFile(GAME_CODE_LIB, "game1.dll", 0);
#endif // __MINGW32__
    handle = LoadLibraryA("game1.dll");
    last_dll_write_time = GetFileModTime("fl");
    game_get_code_dynamic = (game_get_code_t)GetProcAddress(handle, "game_get_code");
#ifdef __MINGW32__
#pragma GCC diagnostic pop
#endif // __MINGW32__
#endif // _WIN32
#if __linux__ || __APPLE__
    handle = dlopen(GAME_CODE_LIB, RTLD_LAZY);
    last_dll_write_time = GetFileModTime(GAME_CODE_LIB);
    *(void **)(&game_get_code_dynamic)  = dlsym(handle, "game_get_code");
#endif // __linux__ || __APPLE__
#ifdef PLATFORM_WEB
    game_code = game_get_code();
#endif // PLATFORM_WEB
    game_code = game_get_code_dynamic();
    game_code->load_code();
}

void game_init(void)
{
    game_code->init();
}

void game_tick()
{
    game_code->tick();
#if _DEBUG || DEBUG_MODE
#ifdef _WIN32
    if (GetFileModTime("fl") != last_dll_write_time)
    {
        game_unload_code();
        game_load_code();
    }
#endif // _WIN32
#if __linux__ || __APPLE__
    if (GetFileModTime(GAME_CODE_LIB) != last_dll_write_time)
    {
        game_unload_code();
        game_load_code();
    }
#endif // __linux__ || __APPLE__
#endif // _DEBUG || DEBUG_MODE
}

void game_shutdown(void)
{
    game_code->shutdown();
    if (handle)
    {
#ifdef _WIN32
        FreeLibrary(handle);
#endif // _WIN32
#if __linux__ || __APPLE__
        dlclose(handle);
#endif // __linux__ || __APPLE__
    }
}

int game_should_continue(void)
{
    return game_code->should_continue();
}

void game_unload_code(void)
{
    game_code->unload_code();
    if (handle)
    {
#ifdef _WIN32
        FreeLibrary(handle);
#endif // _WIN32
#if __linux__ || __APPLE__
        dlclose(handle);
#endif // __linux__ || __APPLE__
        game_code = 0;
    }
}
