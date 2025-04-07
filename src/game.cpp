#include "game.h"

void Game::InitGame() {
    SetTargetFPS(60);
    fire_rate = 15;
    fire_rate_counter = 0;

    defaultBullet.loadAssets() {
        bullet_player = LoadTexture("bullet.png");
    }

    desert.loadAssets() {
        dirt_grass = LoadTexture("dirt_grass.png");
        dirt = LoadTexture("dirt.png");
        bush_spritesheet = LoadTexture("bush_spritesheet");
        path = LoadTexture("path.png");
        bridge = LoadTexture("bridge.png");
        logs = LoadTexture("logs.png");
    }
}

void Game::UpdateGame() {
    if (IsKeyDown(KEY_A)) player.player_pos.x -= 5;
    if (IsKeyDown(KEY_D)) player.player_pos.x += 5;
    if (IsKeyDown(KEY_W)) player.player_pos.y -= 5;
    if (IsKeyDown(KEY_S)) player.player_pos.y += 5;

    if (player.player_pos.x < 0) player.player_pos.x = 0;
    if (player.player_pos.y < 0) player.player_pos.y = 0;
    if (player.player_pos.x > 480) player.player_pos.x = 480;
    if (player.player_pos.y > 480) player.player_pos.y = 480;
}

void Game::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawMap();
    DrawRectangleV(player.player_pos, player.player_size, BLACK);
    if (obstacle.alive) DrawRectangleV(obs.position, { 32, 32 }, obs.color);
    EndDrawing();
}

void Game::UpdateDrawFrame() {
    UpdateGame();
    DrawGame();
}

void Game::UnloadGame() {
    UnloadAssets();
}
