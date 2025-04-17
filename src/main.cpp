#include "window.h"

int main() {
    Window window(32.0f);
    window.load();

    while (!WindowShouldClose()) {
        window.update();
        window.draw();
    }

    window.unload();
    return 0;
}