#include "window.h"
#include "resource_dir.h"

Window::Window(float ts) : tile_size(ts), player(ts) {
    tiles = 16;
    width = tiles * tile_size;
    height = tiles * tile_size;

    is_game_over = false;

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

    std::ifstream map_file("areas/1-1.txt");
    std::string line;
    while (std::getline(map_file, line)) {
        std::string cleaned;

        for (char c : line) {
            if (c != ' ') {
                cleaned += c;
            }
        }
        if (!cleaned.empty()) {
            map_data.push_back(cleaned);
        }
    }
    
    dirt = LoadTexture("sprites/dirt.png");
    path = LoadTexture("sprites/path.png");

    player.load_texture();
}

void Window::unload() {
    CloseAudioDevice();

    UnloadSound(hurt);

    map_data.clear();

    UnloadTexture(dirt);
    UnloadTexture(path);

    CloseWindow();
}

void Window::update() {
    float delta_time = GetFrameTime();

    if (is_game_over) {
        if (IsKeyPressed(KEY_ENTER)) {
            player.reset_health();
            bullets.clear();
            enemies.clear();
            enemy_spawn_timer = 0.0f;
            player.set_position({ width / 2, height / 2 });
            is_game_over = false;
        }
        return;
    }

    player.move(width, height);
    player.update_invincibility(delta_time);
    player.update_texture(delta_time);

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
                spawn_position = { GetRandomValue(7, 9) * tile_size, 0};
                break;
            case 1: // bottom
                spawn_position = { GetRandomValue(7, 9) * tile_size, height - tile_size};
                break;
            case 2: // left
                spawn_position = { 0, GetRandomValue(7, 9) * tile_size};
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
            if (player.take_damage(1)) {
                PlaySound(hurt);
            }
            break;
        }
    }

    if (player.get_health() <= 0) {
        is_game_over = true;
    }
}

void Window::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (is_game_over) {
        DrawText("GAME OVER", width / 2 - 120, height / 2 - 40, 40, RED);
        DrawText("Press ENTER to restart", width / 2 - 120, height / 2 + 10, 20, LIGHTGRAY);
        EndDrawing();
        return;
    }

    for (int y = 0; y < map_data.size(); y++) {
        for (int x = 0; x < map_data[y].size(); x++) {
            char tile = map_data[y][x];
            Vector2 position = { x * tile_size, y * tile_size };

            if (tile == 'D') {
                DrawTextureEx(dirt, position, 0.0f, tile_size / tiles, WHITE);
            }
            else if (tile == 'P') {
                DrawTextureEx(path, position, 0.0f, tile_size / tiles, WHITE);
            }
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