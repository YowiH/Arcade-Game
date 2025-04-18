#include "window.h"

int main() {
    float tile_size = 32.0f;
    Window window(tile_size);
    window.load();

    while (!WindowShouldClose()) {
        window.update();
        window.draw();
    }

    window.unload();
    return 0;
}