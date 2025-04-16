#include "window.h"

void Window::create() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    tile_size = 32;
    width = 16 * tile_size;
    height = 16 * tile_size;
    InitWindow(width, height, "Journey of the Prairie King");
    SetTargetFPS(60);
    fire_rate = 15;
    fire_rate_counter = 0;
    
}

void Window::texturize() {

}

void Window::update() {
    player.move();
}

void Window::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    player.draw();
    
    EndDrawing();
}

void Window::unload() {
    CloseWindow();
}