/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/


#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include <iostream>
using namespace std;

Vector2 player_size{ 16, 16 };
Vector2 player_pos{ 0, 0};
int player_Speed = 5;

int fire_frame_counter = 0;
int fire_rate = 15;

//animation
int animation_frame_counter = 0;

//bush
/*
bool bush_frame = 0;
Rectangle bush_rec = { 0.0f, 0.0f, () };

const float tile_size = 32;
const float area_size = tile_size * 16;
const int NUMBER_OF_TILES = 256;

Vector2 player_size{ tile_size, tile_size };
Vector2 player_pos{ tile_size * 8, tile_size * 3 };
int player_Speed = 1;
Vector2 Shoot_dir;
int damage = 1;*/

struct bullet {
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int damage;
};

struct enemy {
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int hp;
};

struct obstacle {
	bool alive = true;
	Vector2 position;
	Color color;
};

struct obstacle obs;

std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

std::vector<enemy> enemy_pool{};
std::vector<enemy> enemy_pool{};

void InitGame() {
	SetTargetFPS(60);
	obs.color = RED;
	obs.position = Vector2{ tile_size * 8, tile_size * 8 };

}
void UpdateGame() {//update variables and positions
	//command with keys

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

	//enemy movement
	float magnitude = sqrt((player_pos.x - obs.position.x) * (player_pos.x - obs.position.x) + (player_pos.y - obs.position.y) * (player_pos.y - obs.position.y));
	if (obs.alive) {
		obs.position = { obs.position.x + ((player_pos.x - obs.position.x) / magnitude) * 5, obs.position.y + ((player_pos.y - obs.position.y) / magnitude) * 5 };
	}


	if (IsKeyDown(KEY_UP)) {
		Shoot_dir = { Shoot_dir.x, -1 };
	}
	else if (IsKeyDown('D')) {
		Shoot_dir = { Shoot_dir.x , 1 };
	}

	if (IsKeyDown('S')) {
		Shoot_dir = { 1, Shoot_dir.y };
	}
	else if (IsKeyDown('W')) {
		Shoot_dir = { -1, Shoot_dir.y };
	}

	//create bullets
	if ((Shoot_dir - x != 0 || Shoot_dir.y != 0) && frame_counter % fire_rate == 0) {
		if (bullet_pool.empty()) {
			struct bullet b;
			b.damage = damage;
			b.position = { player_pos.x + tile_size / 2, player_pos.y + tile_size / 2 };
			b.velocity = Shoot_dir;
			bullet_tracker.push_back(b);
		}
		else {
			bullet_tracker.push_back(bullet_pool.back());
			bullet_tracker.back().damage = damage;
			bullet_tracker.back().position = { player_pos.x + tile_size / 2 , player_pos.y + tile_size / 2 };
			bullet_tracker.back().velocity = Shoot_dir;
			bullet_pool.pop_back();
		}
		fire_frame_counter++;
	}
	else if (fire_frame_counter % fire_rate != 0) {
		fire_frame_counter++;
	}
	Shoot_dir = { 0, 0 };

	//get the lenght of the bullet vector
	int bullet_amount = bullet_tracker.size();
	//update bullet's position
	for (int i = 0; i < bullet_amount; i++) {
		bullet& b = bullet_tracker[i];
		b.position = Vector2{ b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed };
	}

	//destroy bullets
	for (int i = bullet_amount - 1; i >= 0; i--) {
		if ((bullet_tracker[i].position.x <= 0 || bullet_tracker[i].position.x >= area_size || bullet_tracker[i].position.y <= 0 || bullet_tracker[i].position.x >= area_size)) { //&& bullet_tracker[i] != NULL
			//save the bullet in the pool
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}
	}

	//collisions

	if (CheckCollisionCircles(player_pos, tile_size / 2, obs.position, tile_size / 2) && obs.alive) {
		cout << "you are dead" << endl;
	}

	bullet_amount = bullet_tracker.size();

	for (int i = bullet_amount - 1; i >= 0; i--) {
		if (CheckCollisionCircles(bullet_tracker[i].position, tile_size / 4, obs.position, tile_size / 2) && obs.alive) {
			obs.alive = false;
			//save the bullet in the pool
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}

	}
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
	InitWindow(256, 256, "Hello Raylib");

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
