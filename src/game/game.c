#include "game.h"
#include "raylib.h"
#include "game_state.h"

void draw_rectangle_with_border(int x, int y, int width, int height, Color color, Color border_color);
void draw_game_area_border(void);

void game_init(void)
{
    InitWindow(480, 880, "tetris");

    SetTargetFPS(60);
}

void game_tick(float delta_time)
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
    int block_size = GetScreenWidth() / 12;
    DrawRectangle(0, 0, 12 * block_size, block_size, RED);
    DrawRectangle(0, 21 * block_size, 12 * block_size, block_size, RED);
    DrawRectangle(0, 1 * block_size, block_size, 20 * block_size, RED);
    DrawRectangle(11 * block_size, 1 * block_size, block_size, 20 * block_size, RED);
    
}

void game_shutdown(void)
{
    CloseWindow();
}

int game_should_continue(void)
{
    return !WindowShouldClose();
}

void game_load_code(void)
{
}

void game_unload_code(void)
{
}


game_code_t* game_get_code(void)
{
    static game_code_t game_code = {0};
    game_code.init = game_init;
    game_code.tick = game_tick;
    game_code.shutdown = game_shutdown;
    game_code.should_continue = game_should_continue;
    game_code.load_code = game_load_code;
    game_code.unload_code = game_unload_code;
    return &game_code;
}