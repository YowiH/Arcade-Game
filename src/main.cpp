/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
using namespace std;

#include "raylib.h"
#include <vector>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

Vector2 player_size{ 16, 16 };
Vector2 player_pos{0, 0};
int player_Speed = 2;

struct bullet {
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int damage;
};

std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

//Texture wabbit;

void LoadAssets() {
	//load all assets here please!!
	//wabbit = LoadTexture("wabbit_alpha.png");
}
void UnloadGame() {
	//UnloadTexture(wabbit);
}
void InitGame() {
	SetTargetFPS(60);
	LoadAssets();
}
void UpdateGame() {//update variables and positions
	//MOVEMENT

	if (IsKeyDown('A')) {
		player_pos = { player_pos.x - 5, player_pos.y };
	}
	else if (IsKeyDown('D')) {
		player_pos = { player_pos.x + 5, player_pos.y };
	}

	if (IsKeyDown('S')) {
		player_pos = { player_pos.x, player_pos.y + 5 };
	}
	else if (IsKeyDown('W')) {
		player_pos = { player_pos.x, player_pos.y - 5 };
	}

	//x axis limits
	if (player_pos.x > 240) {
		player_pos = { 240, player_pos.y };
	}
	else if(player_pos.x < 0) {
		player_pos = { 0, player_pos.y };
	}
	//y axis limits
	if (player_pos.y > 240) {
		player_pos = { player_pos.x, 240 };
	}
	else if (player_pos.y < 0) {
		player_pos = { player_pos.x, 0 };
	}

	//SHOOTING BULLETS

	//create bullets
	if (IsKeyDown(KEY_UP)) {
		struct bullet b;
	}

	//destroy bullets

}
void DrawGame() {//draws the game every frame
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawRectangleV(player_pos, player_size, BLACK);
	EndDrawing();
}
void UpdateDrawFrame() {
	while (!WindowShouldClose) {
		UpdateGame();
		DrawGame();
	}
	UnloadGame();
	//CloseWindow();
}

int main ()
{
	//InitGame();
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(256, 256, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	
	

	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}

	// cleanup
	// unload our texture so it can be cleaned up

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
