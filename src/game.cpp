#include "raylib.h"
#include "resource_dir.h"

#include "game.h"

void InitGame() {
    SetTargetFPS(60);
    LoadAssets();
    obs.color = RED;
    obs.position = { 256, 256 };
}

void UpdateGame() {
    if (IsKeyDown(KEY_A)) player_pos.x -= 5;
    if (IsKeyDown(KEY_D)) player_pos.x += 5;
    if (IsKeyDown(KEY_W)) player_pos.y -= 5;
    if (IsKeyDown(KEY_S)) player_pos.y += 5;

    if (player_pos.x < 0) player_pos.x = 0;
    if (player_pos.y < 0) player_pos.y = 0;
    if (player_pos.x > 480) player_pos.x = 480;
    if (player_pos.y > 480) player_pos.y = 480;
}

void DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawMap();
    DrawRectangleV(player_pos, player_size, BLACK);
    if (obs.alive) DrawRectangleV(obs.position, { 32, 32 }, obs.color);
    EndDrawing();
}

void UpdateDrawFrame() {
    UpdateGame();
    DrawGame();
}

void UnloadGame() {
    UnloadAssets();
}
