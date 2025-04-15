#include "raylib.h"
#include "resource_dir.h"

#include "window.h"

int main() {
    Window window;

    window.create();
    window.texturize();

    while (!WindowShouldClose()) {
        window.update();
        window.draw();
    }

    window.unload();
    return 0;
}