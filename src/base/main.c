#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
#include "game.h"

int main(void)
{
    SetTraceLogLevel(LOG_LEVEL);

    game_load_code();

    game_init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(tick, 0, 1);
#else
    while (game_should_continue())
    {
        game_tick(GetFrameTime());
    }
#endif

    game_terminate();

    return 0;
}