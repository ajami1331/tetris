#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int block_size;

void tick(void);
void draw_rectangle_with_border(int x, int y, int width, int height, Color color, Color border_color);
void draw_game_area_border(void);

int main(void)
{
    SetTraceLogLevel(LOG_LEVEL);

    InitWindow(480, 880, "tetris");

    SetTargetFPS(60);

    block_size = GetScreenHeight() / 22;

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(tick, 0, 1);
#else
    while (!WindowShouldClose())
    {
        tick();
    }
#endif

    CloseWindow();

    return 0;
}

void tick(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);
    
    draw_game_area_border();

    EndDrawing();
}

void draw_rectangle_with_border(int x, int y, int width, int height, Color color, Color border_color)
{
    int border_thickness = (width < height ? width : height) / 10;
    DrawRectangleLinesEx((Rectangle){x, y, width, height}, border_thickness, border_color);
    DrawRectangle(x + border_thickness, y + border_thickness, width - (border_thickness * 2), height - (border_thickness * 2), color);
}

void draw_game_area_border(void)
{
    DrawRectangle(0, 0, 12 * block_size, block_size, RED);
    DrawRectangle(0, 21 * block_size, 12 * block_size, block_size, RED);
    DrawRectangle(0, 1 * block_size, block_size, 20 * block_size, RED);
    DrawRectangle(11 * block_size, 1 * block_size, block_size, 20 * block_size, RED);
    int border_thickness = block_size / 10;
    for (int i = 0; i < 22; i++)
    {
        DrawRectangleLinesEx((Rectangle){0, i * block_size, block_size, block_size}, border_thickness, BLACK);
        DrawRectangleLinesEx((Rectangle){11 * block_size, i * block_size, block_size, block_size}, border_thickness, BLACK);
    }
    for (int i = 1; i < 11; i++)
    {
        
        DrawRectangleLinesEx((Rectangle){i * block_size, 0, block_size, block_size}, border_thickness, BLACK);
        DrawRectangleLinesEx((Rectangle){i * block_size, 21 * block_size, block_size, block_size}, border_thickness, BLACK);
    }
}