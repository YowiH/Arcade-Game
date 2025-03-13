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

struct obstacle {
	Vector2 position;
	Color color;
};
struct obstacle obs;

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
	obs.color = RED;
	obs.position = Vector2{ 125, 125 };
	//LoadAssets();
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
		//first look for bullets in the pool
		if (bullet_pool.empty()) {
			struct bullet b;
			b.damage = 1;
			b.position = player_pos;
			b.velocity = { 1, 1 };
			bullet_tracker.push_back(b);
		}
		else {
			bullet_tracker.push_back(bullet_pool.back());
			bullet_tracker.back().damage = 1;
			bullet_tracker.back().position = player_pos;
			bullet_tracker.back().velocity = { 1, 1 };
			bullet_pool.pop_back();
		}
	}

	//get the lenght of the bullet vector
	int bullet_amount = bullet_tracker.size();
	//update bullet's position
	for (int i = 0; i < bullet_amount; i++) {
		bullet& b = bullet_tracker[i];
		b.position = Vector2 { b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed };
	}
	
	//destroy bullets
	for (int i = 0; i < bullet_amount; i++) {
		//iterate and check all bullets if they are ouside of the map (should I check if they colisioned?, might only have to check the first shot if we don't check colisions)
		if ((bullet_tracker[i].position.x <= 0 || bullet_tracker[i].position.x >= 256 || bullet_tracker[i].position.y <= 0 || bullet_tracker[i].position.x >= 256)) { //&& bullet_tracker[i] != NULL
			//save the bullet
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			bullet_tracker.erase(bullet_tracker.begin() + i);
			//i--;
		}
			
	}

}
void DrawGame() {//draws the game every frame
	BeginDrawing();
	//draw background
	ClearBackground(RAYWHITE);

	//draw player
	DrawRectangleV(player_pos, player_size, BLACK);

	//draw obstacle
	DrawRectangleV(obs.position, { 16, 16 }, obs.color);

	//draw bullets
	int bullet_amount = bullet_tracker.size();
	for (int i = 0; i < bullet_amount; i++) {
		DrawRectangleV(bullet_tracker[i].position, { 4, 4 }, BLUE);
	}

	EndDrawing();
}
void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
	/*
	while (!WindowShouldClose) {
		UpdateGame();
		DrawGame();
	}
	UnloadGame();
	*/
	//CloseWindow();
}

int main ()
{
	//InitGame();
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(512, 512, "Journey of the prairie king");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	
	InitGame();

	
	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
	return 0;
	/*
	// cleanup
	// unload our texture so it can be cleaned up

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
	*/
}
