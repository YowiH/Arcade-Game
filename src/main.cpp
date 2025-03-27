/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

void ShowStartScreen();
void ShowDeathScreen();
void RestartGame();

#include "raylib.h"
using namespace std;
#include "resource_dir.h"	
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>  


int fire_frame_counter = 0;
int fire_rate = 15;

//animation
int animation_frame_counter = 0;

//bush
bool bush_frame = 0;

const float tile_size = 32;
const float area_size = tile_size * 16;
const int NUMBER_OF_TILES = 256;

Vector2 player_size{ tile_size, tile_size };
Vector2 player_pos{tile_size*8, tile_size*3};
int player_Speed = 1;
Vector2 Shoot_dir = {0,0};
int damage = 1;
bool isDead = false;
bool gameStarted = false;

// death screen selection
int selectedOption = 0;

//class hierarchy
class GameObject {
	private:
	public:
	Vector2 position;
	Vector2 direction;
	int speed;
};

class player_character : GameObject{
	public: 
	Vector2 shoot_dir;
};


struct bullet{
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int damage;
};

struct enemy{
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int hp;
};

struct obsticle{
	bool alive = true;
	Vector2 position;
	Color color;
};

struct obsticle obs;

//bullet vectors
std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

//enemy vectors
std::vector<enemy> enemy_tracker{};
std::vector<enemy> enemy_pool{};

//start screen
Texture2D titleScreen;


//player texture
Texture2D player_character_spritesheet;
Texture2D player_character_death;
Texture2D bullet_player;

//enemies
Texture2D orc_spritesheet;
Texture2D troll_spritesheet;
Texture2D spike_ball_spritesheet;
Texture2D mushroom_spritesheet;
Texture2D butterfly_spritesheet;
Texture2D mummie_spritesheet;
Texture2D imp_spritesheet;

//cowboy
Texture2D cowboy_spritesheet;
Texture2D cowboy_dialog;

//enemy damage and death animation
Texture2D damage_butterfly;
Texture2D damage_cowboy;
Texture2D damage_imp;
Texture2D damage_mummie;
Texture2D damage_mushroom;
Texture2D damage_troll;
Texture2D damage_spyke_ball;
Texture2D death_animation_dungeon;
Texture2D death_animation_enemy;
Texture2D flyer_death_animation;

//shop
Texture2D amunation;
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

//cementary terrain
Texture2D cementary_floors;
Texture2D cementary_gravel;
Texture2D cementary_path;
Texture2D stone_floor;
Texture2D tombstone;

//power ups
Texture2D coffe;
Texture2D coin;
Texture2D coin_5;
Texture2D extra_life;

//Texture wabbit;
void LoadAssets(){
	dirt_grass = LoadTexture("dirt_grass.png");
	dirt = LoadTexture("dirt.png");
	bush_spritesheet = LoadTexture("bush_spritesheet.png");
	path = LoadTexture("path.png");
	bridge = LoadTexture("bridge.png");
	bullet_player = LoadTexture("bullet.png");
	titleScreen = LoadTexture("title_screen.png");

	if (titleScreen.width == 0 || titleScreen.height == 0) {
    std::cout << "Error loading title screen!" << std::endl;
	}

}

void UnloadGame(){
	UnloadTexture(dirt_grass);
	UnloadTexture(dirt);
	UnloadTexture(bush_spritesheet);
	UnloadTexture(path);
	UnloadTexture(titleScreen);
}

void InitGame() {
	SetTargetFPS(60);
	obs.color = RED;
	obs.position = Vector2{tile_size*8, tile_size *8};
	LoadAssets();
}
void UpdateGame() {

	if (!gameStarted) {
        ShowStartScreen();
        return;
    }
    
    if (isDead) {
        ShowDeathScreen();
        return;
    }

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
	else if(player_pos.x < 0) {
		player_pos = { 0, player_pos.y };
	}
	//y axis limits
	if (player_pos.y > area_size - tile_size) {
		player_pos = { player_pos.x, area_size - tile_size };
	}
	else if (player_pos.y < 0) {
		player_pos = { player_pos.x, 0 };
	}

	//enemy
	float magnitude = sqrt((player_pos.x - obs.position.x) * (player_pos.x - obs.position.x) + (player_pos.y - obs.position.y) * (player_pos.y - obs.position.y));
	if(obs.alive){
		obs.position = {obs.position.x + ((player_pos.x - obs.position.x)/magnitude) * 2, obs.position.y + ((player_pos.y - obs.position.y)/magnitude) * 2};

	}

	//shooting bullets
	if (IsKeyDown(KEY_UP)) {
		Shoot_dir.y = -1;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		Shoot_dir.y = 1;
	}

	if (IsKeyDown(KEY_RIGHT)) {
		Shoot_dir.x = 1;
	}
	else if (IsKeyDown(KEY_LEFT)) {
		Shoot_dir.x = -1;
	}

	//create bullets
	if((Shoot_dir.x != 0 || Shoot_dir.y != 0) && fire_frame_counter % fire_rate == 0){
		if(bullet_pool.empty()){
			struct bullet b;
			b.damage = damage;
			b.position = {player_pos.x + tile_size/2, player_pos.y + tile_size/2};
			b.velocity = {Shoot_dir.x * b.speed, Shoot_dir.y * b.speed};
			bullet_tracker.push_back(b);
		} else{
			bullet_tracker.push_back(bullet_pool.back());
			bullet_tracker.back().damage = damage;
			bullet_tracker.back().position = {player_pos.x + tile_size/2, player_pos.y + tile_size/2};
			bullet_tracker.back().velocity = Shoot_dir;
			bullet_pool.pop_back();

		}
		fire_frame_counter++;

	} else if(fire_frame_counter % fire_rate != 0){
		fire_frame_counter++;
	}
	Shoot_dir = {0,0};

	int bullet_amount = bullet_tracker.size();

	//bullet position
	for(int i = 0; i < bullet_amount; i++){
		bullet& b = bullet_tracker[i];
		b.position = Vector2{b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed};
	}

	//destroy bullet
	for(int i = bullet_amount - 1; i >= 0; i--){
		if((bullet_tracker[i].position.x <=0 || bullet_tracker[i].position.x >= area_size || bullet_tracker[i].position.y <= 0 || bullet_tracker[i].position.x >= area_size)){
			bullet_pool.push_back(bullet_tracker[i]);

			bullet_tracker.erase(bullet_tracker.begin() + i);
		}
	}

	//colisions
	//player-enemies
	if(CheckCollisionCircles(player_pos, tile_size/2, obs.position, tile_size/2) && obs.alive){
		isDead = true;
	}

	bullet_amount = bullet_tracker.size();

	for(int i = bullet_amount - 1; i >= 0; i--){
		if(CheckCollisionCircles(bullet_tracker[i].position, tile_size/4, obs.position, tile_size/2)){
			obs.alive = false;
			bullet_pool.push_back(bullet_tracker[i]);
			bullet_tracker.erase(bullet_tracker.begin() + i);
		}
	}

	//animation
	if(animation_frame_counter >= 60){
		animation_frame_counter = 0;
		bush_frame = !bush_frame;
	}

}

void DrawMap(){
	char T;
	ifstream area("resources/AREAS/are1_1.txt");
	Rectangle src;
	if(bush_frame == 0){
		src = {0.0f,0.0f,16.0f,16.0f};
	} else {
		src = {17.0f,0.0f,32.0f,16.0f};
	}
	for(int i = 0; i < 16; i++){
		for(int j = 0; j<16; j++){
			area.get(T);
			switch(T){
			case 'D':
				DrawTextureEx(dirt, {tile_size * j, tile_size*i}, 0, tile_size/16, WHITE);
				break;
			case 'P':
				DrawTextureEx(path, {tile_size * j, tile_size*i}, 0, tile_size/16, WHITE);
				break;
			case 'B':
				break;
			default:
				break;
			}
		}
	}
}
void DrawGame() {//draws the game every frame
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawMap();
	DrawRectangleV(player_pos, player_size, BLACK);
	if(obs.alive){
		DrawRectangleV(obs.position, {tile_size, tile_size}, obs.color);
	}
	int bullet_amount = bullet_tracker.size();
	for (int i = 0; i<bullet_amount; i++) {
		DrawTextureEx(bullet_player, bullet_tracker[i].position, 0, tile_size / 16, WHITE);
	}
	EndDrawing();
}

void ShowStartScreen() {
    static float zoom = 0.05f;
	static bool zoomCompleted = false;
	static float zoomSpeed = 0.01f;
	static bool timerStarted = false;
	static float timer = 0.0f;

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextureEx(titleScreen, 
	{area_size / 2 - (titleScreen.width * zoom) / 2, area_size / 2 - (titleScreen.height * zoom) / 2}, 0, zoom, WHITE);
	if (!zoomCompleted) {
		if(zoom < 0.5f){
			zoom += zoomSpeed;
		} else {
			zoomCompleted = true;
			timerStarted = true;
		}
    }
	if (zoomCompleted && timerStarted) {
        timer += GetFrameTime();  // Increment the timer by the frame time (time passed per frame)
        if (timer >= 4.0f) {  // 5 seconds elapsed
            gameStarted = true;
        }
    }

	EndDrawing();
	
}

void ShowDeathScreen() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Restart", area_size / 2 - 50, area_size / 2 + 40, 20, (selectedOption == 0) ? RED : WHITE);
    DrawText("Quit", area_size / 2 - 50, area_size / 2 + 80, 20, (selectedOption == 1) ? RED : WHITE);

	Vector2 arrowPos = {area_size / 2 - 70, area_size / 2 + 40 + (selectedOption * 40)};
    DrawTriangle(arrowPos, {arrowPos.x + 10, arrowPos.y - 5}, {arrowPos.x + 10, arrowPos.y + 5}, YELLOW);
    
    if (IsKeyPressed(KEY_DOWN)) selectedOption = 1;
    if (IsKeyPressed(KEY_UP)) selectedOption = 0;
    if (IsKeyPressed(KEY_ENTER)) {
        if (selectedOption == 0) RestartGame();
        else{
			UnloadGame();
			CloseWindow();
		} 
    }
    EndDrawing();
} 

void RestartGame() {
	UnloadGame(); 
	InitGame();   
    player_pos = {tile_size * 8, tile_size * 3};
    isDead = false;
    gameStarted = true;
}


void UpdateDrawFrame() {
	if (!gameStarted) {
        ShowStartScreen(); 
        return;  
    }

    if (isDead) {
        ShowDeathScreen(); 
        return;
    }
	
	UpdateGame();
	DrawGame();
}

int main ()
{
	//InitGame();
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(area_size, area_size, "Journey of the Prairie King");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("../../resources");
	
	InitGame();

	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
	// destroy the window and cleanup the OpenGL context

	UnloadGame();
    CloseWindow();
    return 0;

}