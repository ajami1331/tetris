#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void update_draw_frame(void);

int main(void)
{
    SetTraceLogLevel(LOG_LEVEL);

    InitWindow(800, 500, "Raylib Cmake C Template");

    SetTargetFPS(60);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_draw_frame, 0, 1);
#else
    while (!WindowShouldClose())
    {
        update_draw_frame();
    }
#endif

    CloseWindow();

    return 0;
}

void update_draw_frame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}
