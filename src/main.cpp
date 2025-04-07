#include "raylib.h"
#include "game.h"
#include "resource_dir.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(512, 512, "Journey of the Prairie King");
    SearchAndSetResourceDir("resources");
    Game::InitGame();

    while (!WindowShouldClose()) {
        Game::UpdateDrawFrame();
    }

    Game::UnloadGame();
    CloseWindow();
    return 0;
}
