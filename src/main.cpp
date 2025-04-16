#include "raylib.h"
#include <vector>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>
#include <string>
#include "Map.h"

using namespace std;

//ANIMATIONS
Rectangle src;
int animation_frame_counter = 0;

//bush
bool bush_frame = 0;
//Rectangle bush_rec = { 0.0f, 0.0f, ()};

const float tile_size = 32;
const float area_size = tile_size * 16;
const int NUMBER_OF_TILES = 256;
int randVal; //used to get random values;

//player varaibles
Vector2 player_size{ tile_size, tile_size};
Vector2 player_pos{ tile_size * 8, tile_size * 3 };
Vector2 player_mov_dir{0, 0};
int player_Speed = 2;
int Mov_dir;
int player_walk_anim_counter = 0;
bool right_foot;
int damage = 1;
bool xPosBlock = false, xNegBlock = false, yPosBlock = false, yNegBlock = false;

//shooting
int fire_frame_counter = 0;
int fire_rate = 15;
Vector2 Shoot_dir;
int anim_dir; //variable to know what animation to play when shooting


//MAPS AND AREAS
Map active_map;

//ENEMY SPAWNING VARIABLES
int active_enemies, max_active_enemies = 10;
int enemy_creation_delay = 30, frames_since_enemy_spawn = 0;


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
class Enemy : GameObject {
	int hp;
};
class orc : Enemy {

};

struct bullet {
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int damage;
};

struct enemy {
	int speed = 1;
	Vector2 velocity;
	Vector2 position;
	int hp;
	int anim_counter = 0;
	bool right_foot;
};

struct trial_obs {
	bool alive = true;
	Vector2 position;
	Color color;
};

struct Obsticle {
	Rectangle rec;
};
struct trial_obs tri;

//bullet vectors
std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

//enemy vectors
std::vector<enemy> enemy_tracker{};
std::vector<enemy> enemy_pool{};

//obstacle vector
std::vector<Obsticle> obsticle_tracker{};
std::vector<Obsticle> obsticle_pool{};

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

//SOUND & MUSIC
//fx
Sound shoot_fx;

//music
Music main_theme;

//DATA MANAGEMENT

void LoadAssets() {
	// MAP TEXTURES
	dirt_grass = LoadTexture("dirt_grass.png");
	dirt = LoadTexture("dirt.png");
	dirt_stones = LoadTexture("dirt_stones.png");
	bush_spritesheet = LoadTexture("bush_spritesheet.png");
	path = LoadTexture("path.png");
	bridge = LoadTexture("bridge.png");
	bullet_player = LoadTexture("bullet.png");
	logs = LoadTexture("logs.png");

	//CHARACTERS
	player_character_spritesheet = LoadTexture("player_character_spritesheet.png");

	//ENEMIES
	orc_spritesheet = LoadTexture("orc_spritesheet.png");

	//SOUND
	shoot_fx = LoadSound("shoot1.mp3");
	//MUSIC
	main_theme = LoadMusicStream("JOTPK_song.wav");

}

void UnloadGame() {
	//textures
	UnloadTexture(dirt_grass);
	UnloadTexture(dirt);
	UnloadTexture(bush_spritesheet);
	UnloadTexture(path);
	UnloadTexture(logs);

	//characters
	UnloadTexture(player_character_spritesheet);

	//enemies
	UnloadTexture(orc_spritesheet);

	//sounds
	UnloadSound(shoot_fx);

	//music
	UnloadMusicStream(main_theme);

	CloseAudioDevice;
}

//INITAILIZE GAME

void InitGame() {
	SetTargetFPS(60);
	active_map = Map("AREAS/area1_1.txt");

	//delete next two lines when done with trials
	tri.color = RED;
	tri.position = Vector2{ tile_size * 8, tile_size * 8 };
	InitAudioDevice();
	LoadAssets();
	//start playing music
	PlayMusicStream(main_theme);
}

void PlayerMovement() {
	//Mov_dir = 0;
	if (IsKeyDown('A') && !xNegBlock) {
		//player_pos = { player_pos.x - player_Speed, player_pos.y };
		player_mov_dir.x = -1;
		Mov_dir = 3;
	}
	else if (IsKeyDown('D') && !xPosBlock) {
		//player_pos = { player_pos.x + player_Speed, player_pos.y };
		player_mov_dir.x = 1;
		Mov_dir = 1;
	}

	if (IsKeyDown('S') && !yPosBlock) {
		//player_pos = { player_pos.x, player_pos.y + player_Speed };
		player_mov_dir.y = 1;
		Mov_dir = 2;
	}
	else if (IsKeyDown('W') && !yNegBlock) {
		//player_pos = { player_pos.x, player_pos.y - player_Speed };
		player_mov_dir.y = -1;
		Mov_dir = 4;
	}

	//in order to make diagonal movements as fast as horizontal and vertical movements we must reduce the velocity in both directions
	if (player_mov_dir.y != 0 && player_mov_dir.x != 0) {
		player_pos = { player_pos.x + player_mov_dir.x * player_Speed * 0.707f, player_pos.y + player_mov_dir.y * player_Speed * 0.707f};
	}
	else{
		player_pos = {player_pos.x + player_mov_dir.x * player_Speed, player_pos.y + player_mov_dir.y * player_Speed};
	}
	
	//x axis limits
	if (player_pos.x > area_size + (tile_size)) {
		player_pos = { area_size + (tile_size), player_pos.y };
	}
	else if (player_pos.x < (tile_size * 4)) {
		player_pos = { (tile_size * 4), player_pos.y };
	}
	//y axis limits
	if (player_pos.y > area_size - tile_size) {
		player_pos = { player_pos.x, area_size - tile_size };
	}
	else if (player_pos.y < tile_size*2) {
		player_pos = { player_pos.x, tile_size*2 };
	}
	player_mov_dir = { 0, 0 };
	//restart block variables
	xPosBlock = false;
	xNegBlock = false;
	yPosBlock = false;
	yNegBlock = false;
}

void createEnemies() {
	if (active_enemies < max_active_enemies && frames_since_enemy_spawn >= enemy_creation_delay) {
		randVal = GetRandomValue(0, 11);
		Vector2 pos = {0, 0};
		frames_since_enemy_spawn = 0;
		switch (randVal) {
			//costat de dalt
		case 0:
			pos = { tile_size * 10, tile_size };
			break;
		case 1:
			pos = { tile_size * 11, tile_size };
			break;
		case 2:
			pos = { tile_size * 12, tile_size };
			break;
			//costat esquerra
		case 3:
			pos = { tile_size * 3, tile_size * 8 };
			break;
		case 4:
			pos = { tile_size * 3, tile_size * 9 };
			break;
		case 5:
			pos = { tile_size * 3, tile_size * 10 };
			break;

			//costat dret
		case 6:
			pos = { tile_size * 19, tile_size * 8 };
			break;
		case 7:
			pos = { tile_size * 19, tile_size * 9 };
			break;
		case 8:
			pos = { tile_size * 19, tile_size * 10 };
			break;
			//costat de baix
		case 9:
			pos = { tile_size * 11,  tile_size * 16 };
			break;
		case 10:
			pos = { tile_size * 12, tile_size * 16 };
			break;
		case 11:
			pos = { tile_size * 13, tile_size * 16 };
			break;
		defalult:
			cout << "the random value has a wrong value" << endl;
		}
		if (enemy_pool.empty()) {
			enemy baddie;
			baddie.position = pos;
			baddie.hp = 1;
			enemy_tracker.push_back(baddie);
		}
		else {
			enemy_tracker.push_back(enemy_pool.back());
			enemy_tracker.back().hp = 1;
			enemy_tracker.back().position = pos;
			enemy_pool.pop_back();
		}
		active_enemies++;
	
	}
	else {
		frames_since_enemy_spawn++;
	}
}

void enemyMovement() {
	//delete this trial code
	float magnitude;
	for (int i = 0; i < enemy_tracker.size(); i++) {
		magnitude = sqrt((player_pos.x - enemy_tracker[i].position.x) * (player_pos.x - enemy_tracker[i].position.x) + (player_pos.y - enemy_tracker[i].position.y) * (player_pos.y - enemy_tracker[i].position.y));
		enemy_tracker[i].position = { enemy_tracker[i].position.x + ((player_pos.x - enemy_tracker[i].position.x) / magnitude) * enemy_tracker[i].speed, enemy_tracker[i].position.y + ((player_pos.y - enemy_tracker[i].position.y) / magnitude) * enemy_tracker[i].speed };
	}
	//magnitude = sqrt((player_pos.x - tri.position.x) * (player_pos.x - tri.position.x) + (player_pos.y - tri.position.y) * (player_pos.y - tri.position.y));
	/*if (tri.alive) {
		tri.position = { tri.position.x + ((player_pos.x - tri.position.x) / magnitude) * 2, tri.position.y + ((player_pos.y - tri.position.y) / magnitude) * 2 };
	}*/
}

//BULLETS MANAGER

void bulletShooting() {
	Shoot_dir = { 0, 0 };
	//we get the direction of the bullet
	if (IsKeyDown(KEY_UP)) {
		Shoot_dir = { Shoot_dir.x, -1 };
		anim_dir = 4;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		Shoot_dir = { Shoot_dir.x, 1 };
		anim_dir = 2;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		Shoot_dir = { 1, Shoot_dir.y };
		anim_dir = 1;
	}
	else if (IsKeyDown(KEY_LEFT)) {
		Shoot_dir = { -1, Shoot_dir.y };
		anim_dir = 3;
	}

	//create bullets
	if ((Shoot_dir.x != 0 || Shoot_dir.y != 0) && fire_frame_counter % fire_rate == 0) {
		//first look for bullets in the pool
		PlaySound(shoot_fx);
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
	//Shoot_dir = { 0, 0 };
}

void bulletUpdate(int bullet_amount) {
	//get the lenght of the bullet vector
	//update bullet's position
	for (int i = 0; i < bullet_amount; i++) {
		bullet& b = bullet_tracker[i];
		b.position = Vector2{ b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed };
	}

	//destroy bullets
	for (int i = bullet_amount - 1; i >= 0; i--) {
		//iterate and check all bullets if they are ouside of the map (should I check if they colisioned?, might only have to check the first shot if we don't check colisions)
		if ((bullet_tracker[i].position.x <= tile_size * 3 || bullet_tracker[i].position.x >= area_size + (tile_size * 3) || bullet_tracker[i].position.y <= tile_size || bullet_tracker[i].position.y >= area_size + tile_size)) { //&& bullet_tracker[i] != NULL
			//save the bullet in the pool
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}

	}
}

//COLLISIONS
void player_enemyColl() {
	for (int i = 0; i < enemy_tracker.size(); i++) {
		if (CheckCollisionCircles(player_pos, tile_size / 2, enemy_tracker[i].position, tile_size / 2)) {
			cout << "you are dead" << endl;
		}
	}
	/*if (CheckCollisionCircles(player_pos, tile_size / 2, tri.position, tile_size / 2) && tri.alive) {
		cout << "you are dead" << endl;
	}*/
	
}

void restrainPlayerMovement(float obsPosX, float obsPosY) {
	//bloquejar mov dreta
	if (player_pos.x <= obsPosX) { //pitagoras bby
		xPosBlock = true;
	}
	//bloquejar mov esquerra
	else if (player_pos.x >= obsPosX) {
		xNegBlock = true;
	}
	//bloquejar mov down
	else if (player_pos.y <= obsPosY) {
		yPosBlock = true;
	}
	//bloquejar mov up
	else {
		yNegBlock = true;
	}


}

void player_obsticleColl() {
	//check all obsticle colliders
	Rectangle rec_player = { player_pos.x, player_pos.y, tile_size, tile_size };
	for (int i = 0; i < obsticle_tracker.size(); i++) {
		if (CheckCollisionRecs(rec_player, obsticle_tracker[i].rec)) {
			restrainPlayerMovement(obsticle_tracker[i].rec.x, obsticle_tracker[i].rec.y);
		}
	}
}

void bullet_enemyColl() { //bug here?
	//comprovar totes les bales per tots els enemics
	for (int i = 0; i < enemy_tracker.size(); i++) {
		for (int j = bullet_tracker.size() - 1; j >= 0; j--) {
			if (CheckCollisionCircles(bullet_tracker[j].position, tile_size / 4, enemy_tracker[i].position, tile_size / 2)) {
				//save the bullet in the pool
				bullet_pool.push_back(bullet_tracker[j]);
				//borrar bullet
				auto& k = bullet_tracker.begin() + j;
				bullet_tracker.erase(k);

				//reduce hit enemy hitpoints
				enemy_tracker[i].hp -= damage;
				//kill enemy if hitpoints are 0 or lower
				if (enemy_tracker[i].hp <= 0) {
					//save the enemy in the pool
					enemy_pool.push_back(enemy_tracker[i]);
					//borrar enemy
					auto& e = enemy_tracker.begin() + i;
					enemy_tracker.erase(e);
					active_enemies--;
					
				}
			}
		}
	}
}

void bullet_obsticleColl() { //bug here?
	for (int j = 0; j < obsticle_tracker.size(); j++) {
		for (int i = bullet_tracker.size() - 1; i >= 0; i--) {

			if (CheckCollisionCircles(bullet_tracker[i].position, tile_size / 4, { obsticle_tracker[j].rec.x + tile_size/2, obsticle_tracker[j].rec.y + tile_size/2}, tile_size / 2)) {
				//save the bullet in the pool
				bullet_pool.push_back(bullet_tracker[i]);
				//borrar bullet
				auto& k = bullet_tracker.begin() + i;
				bullet_tracker.erase(k);
			}
		}
	}
}

//ANIMATION
void animationManager() {
	animation_frame_counter++;
	//MAP

	//bush animation
	if (animation_frame_counter % 60 == 0) {//once per second
		animation_frame_counter = 0;
		bush_frame = !bush_frame;

	}

	//PLAYER
	if (Mov_dir != 0){ //si el player s'està movent
		player_walk_anim_counter++;
		if (player_walk_anim_counter % 6 == 0) {
			right_foot = !right_foot;
		}
	}
	else {
		player_walk_anim_counter = 0;
	}

	//ENEMIES
	for (int i = 0; i < enemy_tracker.size(); i++) {
		enemy_tracker[i].anim_counter++;
		if (enemy_tracker[i].anim_counter % 12 == 0) {
			enemy_tracker[i].right_foot = !enemy_tracker[i].right_foot;
			enemy_tracker[i].anim_counter = 0;
		}
	}
}

void UpdateGame() {//update variables and positions

	//MUSIC
	UpdateMusicStream(main_theme);

	//MOVEMENT
	PlayerMovement();
	//ENEMY MOVEMENT
	enemyMovement();

	//CREATE ENEMIES
	createEnemies();
	//SHOOTING BULLETS
	bulletShooting();

	//update bullets
	bulletUpdate(bullet_tracker.size());

	//COLLISIONS
	//player-enemies
	player_enemyColl();

	//player-obstacles
	player_obsticleColl();
	
	//bullet-enemies
	bullet_enemyColl();
	//bullets-obstacles
	bullet_obsticleColl();

	//ANIMATIONS
	animationManager();
}


void positionObsticle(float posX, float posY) {
	if (obsticle_pool.empty()) {
		Obsticle obs;
		obs.rec = {posX, posY, tile_size, tile_size};
		obsticle_tracker.push_back(obs);
	}
	//afegir codi de bullet pool
}
void DrawPlayer() {
	if (anim_dir != 0) {//esta disparant
		if (Mov_dir != 0) { //s'esta movent
			switch (anim_dir) {
			case 1: //dreta
				if (right_foot) {
					src = { 16 * 6, 0, 16, 16 };
				}
				else {
					src = { 0, 16, 16, 16 };
				}
				break;
			case 2: //avall
				if (right_foot) {
					src = { 16 * 4, 0, 16, 16 };
				}
				else {
					src = { 16 * 3, 0, 16, 16 };
				}
				break;
			case 3: //esquerra
				if (right_foot) {
					src = { 16 * 6, 16, 16, 16 };
				}
				else {
					src = { 16 * 5, 16, 16, 16 };
				}
				break;
			case 4: //amunt 
				if (right_foot) {
					src = { 16 * 2, 16, 16, 16 };
				}
				else {
					src = { 16 * 3, 16, 16, 16 };
				}
				break;
			case 0:
				src = { 0, 0, 16, 16 };
				break;
			}
			DrawTexturePro(player_character_spritesheet, src, { player_pos.x, player_pos.y, player_size.x, player_size.y }, { 0, 0 }, 0, WHITE);
		}
		else {//no s'esta movent
			switch (anim_dir) {
			case 1: //dreta
				src = { 16 * 5, 0, 16, 16 };
				break;
			case 2: //avall
				src = {16 * 2, 0, 16, 16};
				break;
			case 3: //esquerra
				src = { 16 * 4, 16, 16, 16 };
				break;
			case 4: //amunt 
				src = { 16 , 16, 16, 16 };
				break;
			}
			DrawTexturePro(player_character_spritesheet, src, { player_pos.x, player_pos.y, player_size.x, player_size.y }, { 0, 0 }, 0, WHITE);

		}
	}
	else {//no esta disparant
		switch (Mov_dir) {//s'està movent
		case 1: //dreta
			if(right_foot){
				src = { 16 * 6, 0, 16, 16 };
			}
			else {
				src = { 0, 16, 16, 16 };
			}
			break;
		case 2: //avall
			if (right_foot) {
				src = {16 * 4, 0, 16, 16};
			}
			else {
				src = { 16 * 3, 0, 16, 16};
			}
			break;
		case 3: //esquerra
			if (right_foot) {
				src = {16 * 6, 16, 16, 16};
			}
			else {
				src = { 16 * 5, 16, 16, 16};
			}
			break;
		case 4: //amunt 
			if (right_foot) {
				src = {16 * 2, 16, 16, 16};
			}
			else {
				src = { 16 * 3, 16, 16, 16};
			}
			break;
		case 0: //no es mou ni dispara
			src = { 0, 0, 16, 16 };
			break;
		}
		DrawTexturePro(player_character_spritesheet, src, { player_pos.x, player_pos.y, player_size.x, player_size.y }, { 0, 0 }, 0, WHITE);
	}
	
	Shoot_dir = { 0,0 };
	anim_dir = 0;
	Mov_dir = 0;
}

void DrawMap(){
	//DrawTextureEx(dirt_grass, { tile_size * 12, tile_size * 12 }, 0, tile_size / 16, WHITE);
	//const char* text[NUMBER_OF_TILES];
	int k = 0;
	//string M{ LoadFileText("AREAS/area1_1.txt") };
	//Rectangle src;
	if (bush_frame == 0) {
		src = { 0.0f, 0.0f, 16.0f, 16.0f }; //tile_size , tile_size
	}
	else {
		src = { 17.0f, 0.0f, 16.0f, 16.0f }; //tile_size , tile_size
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			switch (active_map.getStr()[k]) {
			case 'D':
				DrawTextureEx(dirt, { tile_size * j + (tile_size * 3), tile_size * i + tile_size}, 0, tile_size / 16, WHITE);
				break;
			case 'M':
				DrawTextureEx(dirt_grass, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'S':
				DrawTextureEx(dirt_stones, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'P':
				DrawTextureEx(path, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'B':
				DrawTexturePro(bush_spritesheet, src, { tile_size * j + (tile_size * 3), tile_size * i + tile_size , tile_size, tile_size}, {0,0}, 0, WHITE);
				break;
			case 'O':
				DrawTextureEx(logs, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				positionObsticle(tile_size * j + (tile_size * 3), tile_size * i + tile_size);
				break;
			default:
				break;
			}
			k++;
		}
	}
}
void DrawEnemies() {
	int enemy_amount = enemy_tracker.size();
	for (int i = 0; i < enemy_amount; i++) {
		if (right_foot) {
			src = { 0, 0, 16, 16 };
		}
		else {
			src = { 16, 0, 16, 16 };
		}
		DrawTexturePro(orc_spritesheet, src, { enemy_tracker[i].position.x, enemy_tracker[i].position.y , tile_size, tile_size }, { 0,0 }, 0, WHITE);
	}
}
void DrawBullets() {
	int bullet_amount = bullet_tracker.size();
	for (int i = 0; i < bullet_amount; i++) {
		DrawTextureEx(bullet_player, bullet_tracker[i].position, 0, tile_size / 16, WHITE);
	}
}

void DrawGame() {//draws the game every frame
	BeginDrawing();
	//draw background
	ClearBackground(BLACK);

	//draw map
	DrawMap();

	//draw player
	DrawPlayer();

	//draw enemies
	DrawEnemies();

	/*if (tri.alive) {
		DrawRectangleV(tri.position, { tile_size, tile_size }, tri.color);
	}*/

	//draw bullets
	DrawBullets();

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
	InitWindow(area_size + (tile_size * 3), area_size + tile_size, "Journey of the prairie king");

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