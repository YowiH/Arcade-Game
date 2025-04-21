#include "raylib.h"
#include "resource_dir.h"

#include <fstream>
#include <vector>
#include <string>

#include "map.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

enum class GameState {
    MENU,
    GAME,
    GAME_OVER
};

int main() {
    // ---------------------------------------------------------- declaration
    float tile_size = 32.0f;
    int tiles = 16;

    float width = tiles * tile_size;
    float height = tiles * tile_size;

    Map map;
    Player player(tile_size, width, height);

    std::vector<Bullet> bullets;

    std::vector<Enemy> enemies;
    float enemy_spawn_timer = 0.0f;
    float enemy_spawn_rate = 2.0f;

    bool is_game_over = false;
    GameState game_state = GameState::MENU;

    // -------------------------------------------------------- load
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "Journey of the Prairie King");
    SetTargetFPS(60);
    SearchAndSetResourceDir("../resources");
    InitAudioDevice();
    SetMasterVolume(1.0f);

    map.load(tile_size);
    player.load();

    while (!WindowShouldClose()) {
        // ------------------------------------------------- update
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        // game state update
        switch (game_state) {

        case GameState::MENU:
            DrawTexture(map.get_jopk_logo(), 120, 100, WHITE);
            DrawText("Press ENTER to start", width / 2 - 50, height - 150, 10, LIGHTGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                game_state = GameState::GAME;
            }
            break;

        case GameState::GAME:

            if (player.get_health() <= 0) {
                game_state = GameState::GAME_OVER;
            }

            map.update(delta_time);
            player.update(tile_size, delta_time, width, height, map);

            // bullet update
            Vector2 direction = player.get_shoot_direction();
            if ((direction.x != 0 || direction.y != 0) && player.can_shoot()) {
                PlaySound(map.get_shoot());
                bullets.push_back(Bullet(tile_size, player.get_center(), direction));
                player.reset_fire_cooldown();
            }
            for (int i = bullets.size() - 1; i >= 0; i--) {
                bullets[i].update(delta_time);
                if (bullets[i].is_off_screen(width, height)) {
                    bullets.erase(bullets.begin() + i);
                }
            }

            // enemy update
            enemy_spawn_timer -= delta_time;
            if (enemy_spawn_timer <= 0.0f) {
                int edge = GetRandomValue(0, 3);
                Vector2 spawn_position;
                switch (edge) {
                case 0: // top
                    spawn_position = { GetRandomValue(7, 9) * tile_size, 0 };
                    break;
                case 1: // bottom
                    spawn_position = { GetRandomValue(7, 9) * tile_size, height - tile_size };
                    break;
                case 2: // left
                    spawn_position = { 0, GetRandomValue(7, 9) * tile_size };
                    break;
                case 3: // right
                    spawn_position = { width - tile_size, GetRandomValue(7, 9) * tile_size };
                    break;
                }
                enemies.push_back(Enemy(tile_size, spawn_position, width, height));
                enemy_spawn_timer = enemy_spawn_rate;
            }
            for (int i = enemies.size() - 1; i >= 0; i--) {
                enemies[i].update(tile_size, player.get_center(), delta_time);

                bool enemy_hit = false;
                for (int j = bullets.size() - 1; j >= 0; j--) {
                    if (enemies[i].check_collision(tile_size, bullets[j].get_position(), bullets[j].get_radius())) {
                        enemy_hit = true;
                        bullets.erase(bullets.begin() + j);
                        break;
                    }
                }
                if (enemy_hit) {
                    PlaySound(map.get_enemy_death());
                    enemies.erase(enemies.begin() + i);
                }
            }
            for (Enemy& enemy : enemies) {
                Rectangle player_rectangle = { player.get_position().x, player.get_position().y, tile_size, tile_size };
                Rectangle enemy_rectangle = { enemy.get_position().x, enemy.get_position().y, tile_size, tile_size };
                if (CheckCollisionRecs(player_rectangle, enemy_rectangle)) {
                    if (player.set_damage(1)) {
                        PlaySound(map.get_hurt());
                        enemies.clear();
                    }
                    break;
                }
            }

            // ---------------------------------------------------------- draw
            map.draw(tile_size, tiles);

            player.draw(tile_size);

            for (Bullet& bullet : bullets) {
                bullet.draw();
            }
            for (Enemy& enemy : enemies) {
                enemy.draw(tile_size);
            }

            DrawText(TextFormat("%d", player.get_health()), 10, 10, 20, WHITE);
            break;

        case GameState::GAME_OVER:
            DrawText("GAME OVER", width / 2 - 120, height / 2 - 40, 40, RED);
            DrawText("Press ENTER to restart", width / 2 - 120, height / 2 + 10, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                player.reset_health();
                bullets.clear();
                enemies.clear();
                enemy_spawn_timer = 0.0f;
                player.set_position({ width / 2, height / 2 });
                game_state = GameState::MENU;
            }
            break;
        }
            EndDrawing();
    }

    // -------------------------------------------------- unload

    CloseWindow();
    return 0;
}