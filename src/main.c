#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define HEIGHT_WITH_BORDER          22
#define WIDTH_WITH_BORDER           12
#define HEIGHT                      20
#define WIDTH                       10

int block_size;
char game_area[HEIGHT][WIDTH] = 
{
    "          ",
    "          ",
    "          ",
    "          ",
    "          ",
    "*  *      ",
    "          ",
    "          ",
    "          ",
    "*         ",
    "          ",
    "          ",
    "          ",
    "          ",
    "          ",
    "*  *      ",
    "          ",
    "          ",
    "          ",
    "*         "
};
void tick(void);
void draw_rectangle_with_border(int x, int y, int width, int height, Color color, Color border_color);
void draw_game_area_border(void);
void draw_game_area(void);

int main(void)
{
    SetTraceLogLevel(LOG_LEVEL);

    InitWindow(480, 880, "tetris");

    SetTargetFPS(60);

    block_size = GetScreenHeight() / HEIGHT_WITH_BORDER;

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
    
    draw_game_area_border();

    draw_game_area();

    EndDrawing();
}

void draw_rectangle_with_border(int x, int y, int width, int height, Color color, Color border_color)
{
    int border_thickness = (width < height ? width : height) / WIDTH;
    DrawRectangleLinesEx((Rectangle){x, y, width, height}, border_thickness, border_color);
    DrawRectangle(x + border_thickness, y + border_thickness, width - (border_thickness * 2), height - (border_thickness * 2), color);
}

void draw_game_area_border(void)
{
    DrawRectangle(0, 0, WIDTH_WITH_BORDER * block_size, block_size, RED);
    DrawRectangle(0, 21 * block_size, WIDTH_WITH_BORDER * block_size, block_size, RED);
    DrawRectangle(0, 1 * block_size, block_size, HEIGHT * block_size, RED);
    DrawRectangle(11 * block_size, 1 * block_size, block_size, HEIGHT * block_size, RED);
    int border_thickness = block_size / WIDTH;
    for (int i = 0; i < HEIGHT_WITH_BORDER; i++)
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

void draw_game_area(void)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (game_area[i][j] == '*')
            {
                draw_rectangle_with_border(j * block_size + block_size, i * block_size + block_size, block_size, block_size, RED, BLACK);
            }
        }
    }
}
