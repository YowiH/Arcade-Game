/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

Vector2 player_size{ 16, 16 };
Vector2 player_pos{0, 0};

void InitGame() {
	SetTargetFPS(60);
}
void UpdateGame() {//update variables and positions
	player_pos = { player_pos.x + 5, player_pos.y };

}
void DrawGame() {//draws the game every frame
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawRectangleV(player_pos, player_size, BLACK);
	EndDrawing();
}
void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}

int main ()
{
	//InitGame();
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	

	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
