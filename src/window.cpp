#include "window.h"

Window::Window(float ts) : tile_size(ts), player(ts) {
    width = tile_size * 16;
    height = tile_size * 16;

    fire_rate = 0.2f;
    fire_cooldown = 0.0f;

    enemy_spawn_timer = 0.0f;
}

void Window::load() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "Journey of the Prairie King");
    SetTargetFPS(60);
}

void Window::update() {
    float delta_time = GetFrameTime();

    player.move(width, height);

    Vector2 direction = player.get_shoot_direction();
    if ((direction.x != 0 || direction.y != 0) && player.can_shoot()) {
        bullets.push_back(Bullet(player.get_center(), direction));
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
        Vector2 spawn_position{ GetRandomValue(0, width), GetRandomValue(0, height) };
        enemies.push_back(Enemy(spawn_position, 100.0f));
        enemy_spawn_timer = 2.0f;
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
            player.take_damage();
            break;
        }
    }
}

void Window::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

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
    CloseWindow();
}