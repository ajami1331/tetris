#include "game.h"
#include "raylib.h"
#include "game_state.h"
#include "tetrimino.h"
#include "tetrimino_definations.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

void draw_rectangle_with_border(int x, int y, int width, int height, Color color, Color border_color);
void draw_game_area_border(void);
void create_tetrimino(void);

game_state_t *game_state = 0;
float fast_drop_rate = 0.25f;
float drop_rate = 0.55f;
float drop_timer = 0.0f;

bool check_tetrimino_collide_y(void)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {            
            if (!game_state->current_tetrimino.blocks[i][j])
            {
                continue;
            }
            int x = (game_state->current_tetrimino.x + i);
            int y = (game_state->current_tetrimino.y + j) + 1;
            if (y < 0)
            {
                continue;
            }
            if (game_state->blocks[x][y] || y > 20)
            {
                create_tetrimino();
                return false;
            }
        }
    }
    return true;
}

void create_tetrimino(void)
{
    TraceLog(LOG_INFO, "create_tetrimino");
    TraceLog(LOG_INFO, "game_state->current_tetrimino.type: %d", game_state->current_tetrimino.type);
    TraceLog(LOG_INFO, "game_state->current_tetrimino.x: %d", game_state->current_tetrimino.x);
    TraceLog(LOG_INFO, "game_state->current_tetrimino.y: %d", game_state->current_tetrimino.y);
    if (game_state->current_tetrimino.type != TETRIMINO_COUNT)
    {
        for (int i = 0; i < 4; i++)
        {
            int x = (game_state->current_tetrimino.x + i);
            for (int j = 0; j < 4; j++)
            {
                int y = (game_state->current_tetrimino.y + j);
                if (!game_state->current_tetrimino.blocks[i][j])
                {
                    continue;
                }
                game_state->blocks[x][y] = 1;
            }
        }
    }
    game_state->current_tetrimino.x = 5;
    game_state->current_tetrimino.y = -3;
    game_state->current_tetrimino.type = rand() % TETRIMINO_COUNT;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            game_state->current_tetrimino.blocks[i][j] = tetrimino_blocks[game_state->current_tetrimino.type][i][j];
        }
    }
}

void draw_current_tetrimino(void)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (!game_state->current_tetrimino.blocks[i][j])
            {
                continue;
            }
            int block_size = game_state->block_size;
            int x = (game_state->current_tetrimino.x + i) * block_size;
            int y = (game_state->current_tetrimino.y + j) * block_size;
            int width = block_size;
            int height = block_size;
            Color color = RED;
            Color border_color = BLACK;
            draw_rectangle_with_border(x, y, width, height, color, border_color);
        }
    }
}

void game_init(game_state_t *state)
{
    srand(time(0));

    InitWindow(480, 880, "tetris");

    SetTargetFPS(60);

    game_state = state;

    game_state->block_size = GetScreenWidth() / 12;

    game_state->current_tetrimino.type = TETRIMINO_COUNT;

    memset(game_state->blocks, 0, sizeof(game_state->blocks));

    create_tetrimino();
}

void game_tick(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);
    
    draw_game_area_border();
    
    drop_timer += GetFrameTime();

    if (drop_timer >= drop_rate && check_tetrimino_collide_y())
    {
        game_state->current_tetrimino.y += 1;
        drop_timer = 0.0f;
    }    
    
    draw_current_tetrimino();

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
    int block_size = game_state->block_size;
    DrawRectangle(0, 0, 12 * block_size, block_size, RED);
    DrawRectangle(0, 21 * block_size, 12 * block_size, block_size, RED);
    DrawRectangle(0, 1 * block_size, block_size, 20 * block_size, RED);
    DrawRectangle(11 * block_size, 1 * block_size, block_size, 20 * block_size, RED);

    for (int i = 0; i <= 10; i++)
    {
        for (int j = 0; j <= 20; j++)
        {            
            if (!game_state->blocks[i][j])
            {
                continue;
            }
            int x = i * block_size;
            int y = j * block_size;
            int width = block_size;
            int height = block_size;
            Color color = RED;
            Color border_color = BLACK;
            draw_rectangle_with_border(x, y, width, height, color, border_color);
        }
    }
    
}

void game_shutdown(void)
{
    CloseWindow();
}

int game_should_continue(void)
{
    return !WindowShouldClose();
}

void game_load_code(game_state_t *state)
{
    game_state = state;
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