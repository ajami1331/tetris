#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#include "game/game.h"
#else
#include "base/game.h"
#endif

#include "game_state.h"

game_state_t game_state;

int main(void)
{
    SetTraceLogLevel(LOG_LEVEL);

#if _DEBUG || DEBUG_MODE
    TraceLog(LOG_INFO, "Debug build!");
#endif // _DEBUG || DEBUG_MODE

    game_load_code(&game_state);

    game_init(&game_state);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_tick, 0, 1);
#else
    while (game_should_continue())
    {
        game_tick();
    }
#endif

    game_shutdown();

    return 0;
}
