#include "raylib.h"
#include "resource_dir.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int fire_frame_counter = 0;
int fire_rate = 15;

//animation
int animation_frame_counter = 0;

//bush
bool bush_frame = 0;
//Rectangle bush_rec = { 0.0f, 0.0f, ()};

const float tile_size = 32;
const float area_size = tile_size * 16;
const int NUMBER_OF_TILES = 256;

Vector2 player_size{ tile_size, tile_size };
Vector2 player_pos{ tile_size * 8, tile_size * 3 };
int player_Speed = 1;
Vector2 Shoot_dir;
int damage = 1;

//CLASS HIERARCHY
class GameObject {
private:
public:
	Vector2 position;
	Vector2 direction;
	int speed;
};
class player_character : GameObject {
public:
	Vector2 shoot_dir;
};
class Bullet : GameObject {
	int damage;
};
class enemy : GameObject {
	int hp;
};
class orc : enemy {

};

struct bullet {
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int damage;
};

struct trial_enemy {
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

//bullet vectors
std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

//enemy vectors
std::vector<enemy> enemy_tracker{};
std::vector<enemy> enemy_pool{};

//TEXTURES
//Create gloval varaibles for all textures so they can be used by the draw function and the load assets function

//player character
Texture2D player_character_spritesheet;
Texture2D player_character_death;
Texture2D bullet_player;

//enemies
Texture2D orc_spritesheet;
Texture2D troll_spritesheet;
Texture2D spyke_ball_spritesheet;
Texture2D mushroom_spritesheet;
Texture2D butterfly_spritesheet;
Texture2D mummie_spritesheet;
Texture2D imp_spritesheet;

//cowboy
Texture2D cowboy_spritesheet;
Texture2D cowboy_dialog;

//enemy damage & death animation
Texture2D damaged_butterfly;
Texture2D damaged_cowboy;
Texture2D damaged_imp;
Texture2D damaged_mummie;
Texture2D damaged_mushroom;
Texture2D damaged_troll;
Texture2D damaged_spyke_ball;
Texture2D death_animation_dungeon;
Texture2D death_animation_enemy;
Texture2D flyer_death_animation;

//shop
Texture2D ammunition;
Texture2D guns;
Texture2D boots;
Texture2D shop_jeeper_spritesheet;
Texture2D shop_blanket;

//general terrain
Texture2D bridge;

//desert terrain
Texture2D dirt_grass;
Texture2D dirt;
Texture2D dirt_stones;
Texture2D path;
Texture2D path_stones;
Texture2D river_desert;
Texture2D bush_spritesheet;
Texture2D logs;

//forest terrain
Texture2D grass;
Texture2D flowers_grass;
Texture2D path_grass;
Texture2D path_grass2;
Texture2D tall_grass;
Texture2D river_forest;
Texture2D green_bush_spritesheet;
Texture2D stump;

//cementery terrain
Texture2D cementery_floor;
Texture2D cementery_gravel;
Texture2D cementery_path;
Texture2D stone_floor;
Texture2D tombstone;

//power ups
Texture2D coffe;
Texture2D coin;
Texture2D coin_5;
Texture2D extra_life;

//UI
//Texture2D clock;


void LoadAssets() {
	//load all assets here please!!
	dirt_grass = LoadTexture("dirt_grass.png");
	dirt = LoadTexture("dirt.png");
	bush_spritesheet = LoadTexture("bush_spritesheet");
	path = LoadTexture("path.png");
	bridge = LoadTexture("bridge.png");
	bullet_player = LoadTexture("bullet.png");
	logs = LoadTexture("logs.png");
}
void UnloadGame() {
	UnloadTexture(dirt_grass);
	UnloadTexture(dirt);
	UnloadTexture(bush_spritesheet);
	UnloadTexture(path);
}
void InitGame() {
	SetTargetFPS(60);
	obs.color = RED;
	obs.position = Vector2{ tile_size * 8, tile_size * 8 };
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
	if (player_pos.x > area_size - tile_size) {
		player_pos = { area_size - tile_size, player_pos.y };
	}
	else if (player_pos.x < 0) {
		player_pos = { 0, player_pos.y };
	}
	//y axis limits
	if (player_pos.y > area_size - tile_size) {
		player_pos = { player_pos.x, area_size - tile_size };
	}
	else if (player_pos.y < 0) {
		player_pos = { player_pos.x, 0 };
	}

	//ENEMY MOVEMENT
	//delete this trial code

	float magnitude = sqrt((player_pos.x - obs.position.x) * (player_pos.x - obs.position.x) + (player_pos.y - obs.position.y) * (player_pos.y - obs.position.y));
	if (obs.alive) {
		obs.position = { obs.position.x + ((player_pos.x - obs.position.x) / magnitude) * 2, obs.position.y + ((player_pos.y - obs.position.y) / magnitude) * 2 };
	}

	//SHOOTING BULLETS
	//we get the direction of the bullet
	if (IsKeyDown(KEY_UP)) {
		Shoot_dir = { Shoot_dir.x, -1 };
	}
	else if (IsKeyDown(KEY_DOWN)) {
		Shoot_dir = { Shoot_dir.x, 1 };
	}
	if (IsKeyDown(KEY_RIGHT)) {
		Shoot_dir = { 1, Shoot_dir.y };
	}
	else if (IsKeyDown(KEY_LEFT)) {
		Shoot_dir = { -1, Shoot_dir.y };
	}

	//create bullets
	if ((Shoot_dir.x != 0 || Shoot_dir.y != 0) && fire_frame_counter % fire_rate == 0) {
		//first look for bullets in the pool
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
	cout << bullet_amount;
	//update bullet's position
	for (int i = 0; i < bullet_amount; i++) {
		bullet& b = bullet_tracker[i];
		b.position = Vector2{ b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed };
	}

	//destroy bullets
	for (int i = bullet_amount - 1; i >= 0; i--) {
		//iterate and check all bullets if they are ouside of the map (should I check if they colisioned?, might only have to check the first shot if we don't check colisions)
		if ((bullet_tracker[i].position.x <= 0 || bullet_tracker[i].position.x >= area_size || bullet_tracker[i].position.y <= 0 || bullet_tracker[i].position.x >= area_size)) { //&& bullet_tracker[i] != NULL
			//save the bullet in the pool
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}

	}

	//COLLISIONS
	//player-enemies
	if (CheckCollisionCircles(player_pos, tile_size / 2, obs.position, tile_size / 2) && obs.alive) {
		cout << "you are dead" << endl;
	}
	//player-obstacles
	if (CheckCollisionRecs(Rectangle{ player_pos.x, player_pos.y, tile_size, tile_size },
		Rectangle{ obs.position.x, obs.position.y, tile_size, tile_size }) && obs.alive) {
		cout << "Player collided with obstacle" << endl;
		// Ensure player stays within bounds
		if (player_pos.x > area_size - tile_size) {
			player_pos = { area_size - tile_size, player_pos.y };
		}
		else if (player_pos.x < 0) {
			player_pos = { 0, player_pos.y };
		}

		if (player_pos.y > area_size - tile_size) {
			player_pos = { player_pos.x, area_size - tile_size };
		}
		else if (player_pos.y < 0) {
			player_pos = { player_pos.x, 0 };
		}
	}

	//bullet-enemies
	//bullets-obstacles

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

	//ANIMATIONS
	animation_frame_counter++;
	//MAP

	//bush animation
	if (animation_frame_counter >= 60) {//once per second
		animation_frame_counter = 0;
		bush_frame = !bush_frame;

	}
}

void DrawMap() {
	//DrawTextureEx(dirt_grass, { tile_size * 12, tile_size * 12 }, 0, tile_size / 16, WHITE);
	//const char* text[NUMBER_OF_TILES];
	char T;
	ifstream area("AREAS/area1_1.txt");
	Rectangle src;
	if (bush_frame == 0) {
		src = { 0.0f, 0.0f, 16.0f, 16.0f };
	}
	else {
		src = { 17.0f, 0.0f, 32.0f, 16.0f };
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			area.get(T);
			switch (T) {
			case 'D':
				DrawTextureEx(dirt, { tile_size * j, tile_size * i }, 0, tile_size / 16, WHITE);
				break;
			case 'P':
				DrawTextureEx(path, { tile_size * j, tile_size * i }, 0, tile_size / 16, WHITE);
				break;
			case 'B':
				//DrawTextureEx(bush_spritesheet, { tile_size * j, tile_size * i }, 0, tile_size / 16, WHITE);
				//DrawTexturePro(bush_spritesheet, src, { 0.0f, 0.0f, tile_size, tile_size }, { tile_size * j, tile_size * i }, 0, WHITE);
				//DrawTexturePro(bridge, { 0.0f, 0.0f, 16.0f, 16.0f }, { 0.0f, 0.0f, tile_size, tile_size }, { tile_size * j, tile_size * i }, 0, WHITE);
				//DrawTexturePro(bridge, { 0.0f, 0.0f, 16.0f, 16.0f }, { tile_size * j, tile_size * i }, {tile_size, tile_size }, 0, WHITE);
				break;
			case 'L':
				DrawTextureEx(logs, { tile_size * j, tile_size * i }, 0, tile_size / 16, WHITE);
			default:
				break;
			}
		}
	}
}

void DrawGame() {//draws the game every frame
	BeginDrawing();
	//draw background
	ClearBackground(RAYWHITE);

	//draw map
	DrawMap();

	//draw player
	DrawRectangleV(player_pos, player_size, BLACK);

	//draw obstacle
	if (obs.alive) {
		DrawRectangleV(obs.position, { tile_size, tile_size }, obs.color);
	}

	//draw bullets
	int bullet_amount = bullet_tracker.size();
	for (int i = 0; i < bullet_amount; i++) {
		DrawTextureEx(bullet_player, bullet_tracker[i].position, 0, tile_size / 16, WHITE);
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

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(area_size, area_size, "Journey of the prairie king");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");


	//start some values
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