#include "window.h"
#include "resource_dir.h"

Window::Window(float ts) : tile_size(ts), player(ts) {
    tiles = 16;
    width = tiles * tile_size;
    height = tiles * tile_size;

    fire_rate = 0.2f;
    fire_cooldown = 0.0f;

    enemy_spawn_timer = 0.0f;
    enemy_spawn_rate = 2.0f;
}

void Window::load() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "Journey of the Prairie King");
    SetTargetFPS(60);

    InitAudioDevice();
    SetMasterVolume(1.0f);
    SearchAndSetResourceDir("../resources");

    hurt = LoadSound("audio/FX/hurt.wav");

    dirt = LoadTexture("sprites/dirt.png");
}

void Window::update() {
    float delta_time = GetFrameTime();

    player.move(width, height);
    player.update_invincibility(delta_time);

    Vector2 direction = player.get_shoot_direction();
    if ((direction.x != 0 || direction.y != 0) && player.can_shoot()) {
        bullets.push_back(Bullet(tile_size, player.get_center(), direction));
        player.reset_fire_cooldown();
    }
    for (int i = bullets.size() - 1; i >= 0; i--) {
        bullets[i].move(delta_time);
        if (bullets[i].is_off_screen(width, height)) {
            bullets.erase(bullets.begin() + i);
        }
    }

    enemy_spawn_timer -= delta_time;
    if (enemy_spawn_timer <= 0.0f) {
        int edge = GetRandomValue(0, 3);
        Vector2 spawn_position;
        switch (edge) {
            case 0: // top
                spawn_position = { GetRandomValue(8, 10) * tile_size, 0};
                break;
            case 1: // bottom
                spawn_position = { GetRandomValue(8, 10) * tile_size, height - tile_size};
                break;
            case 2: // left
                spawn_position = { 0, GetRandomValue(8, 10) * tile_size};
                break;
            case 3: // right
                spawn_position = { width - tile_size, GetRandomValue(8, 10) * tile_size};
            }
        enemies.push_back(Enemy(tile_size, spawn_position));
        enemy_spawn_timer = enemy_spawn_rate;
    }
    for (int i = enemies.size() - 1; i >= 0; i--) {
        enemies[i].move(player.get_center(), delta_time);

        bool enemy_hit = false;
        for (int j = bullets.size() - 1; j >= 0; j--) {
            if (enemies[i].check_collision(bullets[j].get_position(), bullets[j].get_radius())) {
                enemy_hit = true;
                bullets.erase(bullets.begin() + j);
                break;
            }
        }
        if (enemy_hit) {
            enemies.erase(enemies.begin() + i);
        }
    }
    for (Enemy& enemy : enemies) {
        Rectangle player_rectangle = { player.get_position().x, player.get_position().y, player.get_tile_size(), player.get_tile_size() };
        Rectangle enemy_rectangle = { enemy.get_position().x, enemy.get_position().y, enemy.get_size(), enemy.get_size() };
        if (CheckCollisionRecs(player_rectangle, enemy_rectangle)) {
            if (player.take_damage(20)) {
                PlaySound(hurt);
            }
            break;
        }
    }
}

void Window::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            DrawTextureEx(dirt, { x * tile_size, y * tile_size }, 0.0f, tile_size / 16.0f, WHITE);
        }
    }

    player.draw();

    for (Bullet& bullet : bullets) {
        bullet.draw();
    }

    for (Enemy& enemy : enemies) {
        enemy.draw();
    }

    DrawText(TextFormat("Health %d", player.get_health()), 10, 10, 20, RED);
    
    EndDrawing();
}

void Window::unload() {
    CloseAudioDevice();

    UnloadSound(hurt);

    UnloadTexture(dirt);

    CloseWindow();
}