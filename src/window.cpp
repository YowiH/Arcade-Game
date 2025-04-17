#include "window.h"

void Window::load() {
    tile_size = 32;
    width = 16 * tile_size;
    height = 16 * tile_size;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "Journey of the Prairie King");
    SetTargetFPS(60);

    fire_rate = 0.2f;
    fire_cooldown = 0.0f;
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
}

void Window::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    player.draw();

    for (Bullet& bullet : bullets) {
        bullet.draw();
    }
    
    EndDrawing();
}

void Window::unload() {
    CloseWindow();
}