#include "window.h"

int main() {
    Window window;
    window.load();

    while (!WindowShouldClose()) {
        window.update();
        window.draw();
    }

    window.unload();
    return 0;
}