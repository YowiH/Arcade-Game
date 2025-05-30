#include "raylib.h"
#include <vector>
#include "resource_dir.h"
#include <iostream>
#include <string>
#include "map.h"
#include "obstacle.h"

using namespace std;

//ANIMATIONS
Rectangle src;
int animation_frame_counter = 0;

//bush
bool bush_frame = 0;
//Rectangle bush_rec = { 0.0f, 0.0f, ()};

//CENTRAL VARAIBLES
bool game_started = false;
const float tile_size = 32;
const float area_size = tile_size * 16;
const float left_margin = tile_size * 3;
const float top_margin = tile_size;
const int NUMBER_OF_TILES = 256;
int randVal; //used to get random values;
float level_length = 60 * 10; //all the frames in 2 minutes: 60 * 60 * 2
float frames_since_level_start = 0;
Font font;

static float zoom = 0.01f * (tile_size / 16);
static bool zoom_completed = false;
static float zoom_speed = 0.02f * (tile_size / 16);

//game over screen variables
//int selected_option = 0;
bool close_game;
bool game_won = false;

//player varaibles
Vector2 player_size{ tile_size, tile_size};
Vector2 player_pos{ left_margin + (area_size - player_size.x) / 2 + 16, top_margin + (area_size - player_size.y) / 2 + 16 };
Vector2 player_mov_dir{0, 0};
float player_Speed = 2;
int Mov_dir;
int player_walk_anim_counter = 0;
int player_death_anim = 0;
bool player_right_foot;
bool player_dying;
int lives = 3;
int coins = 0;
int damage = 1;
bool xPosBlock = false, xNegBlock = false, yPosBlock = false, yNegBlock = false;

//shooting
int fire_frame_counter = 0;
int fire_rate = 15;
Vector2 Shoot_dir;
int anim_dir; //variable to know what animation to play when shooting
int boss_shoot_cooldown = 0;

//MAPS AND AREAS
int level_count = 0;
int powerUp_lifespan = 600;
bool obstacles_positioned = false;

//ENEMY SPAWNING VARIABLES
int active_enemies, max_active_enemies = 5;
int enemy_creation_delay = 30, frames_since_enemy_spawn = 0;
bool boss_defeated = false;

//power-ups
char stored_power_up = ' ';
char active_power_up_type = ' ';
int power_up_timer = 0;

//shop
bool shop_active = false;
bool shop_item_bought = false;
int shop_phase = 0; // 0 = inactive, 1 = entering, 2 = idle/shop, 3 = exiting
int shopkeeper_frame = 0;
Vector2 shopkeeper_pos = { (area_size / 2) + left_margin, tile_size };
Texture2D item_sprites[3]; // 0 = boot, 1 = gun, 2 = backpack/ammo
float hold_up_timer = 0;
int last_shop_level = -1;
bool backpack_bought[3] = { false, false, false };
int backpack_level = 0;
int gun_level = 0;
int item_being_held_up = -1;  // -1 = none
bool boot_bought = false;
int gun_upgrade_level = -1;       // -1 means not bought yet
int backpack_upgrade_level = -1;  // -1 means not bought yet
float saved_speed = 0;
int bought_gun_sprite = -1;
int bought_backpack_sprite = -1;

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

struct bullet {
	int speed = 5;
	Vector2 velocity;
	Vector2 position;
	int damage;
	bool isBossBullet = false;
};

struct Enemy {
	float speed;
	Vector2 velocity;
	Vector2 position;
	int hp;
	int anim_counter = 0;
	bool right_foot;
	char type; // 'O' for orc, 'G' for ogre, 'M' for mushroom
	bool isBoss = false;
	int boss_direction = -1;
	int boss_max_left = left_margin + tile_size;
	int boss_max_right = area_size + left_margin - 2 * tile_size;
	int boss_center = left_margin + (area_size - player_size.x) / 2 + 16;
	int boss_min_left = left_margin + (area_size - player_size.x) / 2 - 16;
	int boss_min_right = left_margin + (area_size - player_size.x) / 2 + 3 * 16;
};

struct powerUp {
	Vector2 position;
	char type;
	int despawn_timer = 0;
};

struct trial_obs {
	bool alive = true;
	Vector2 position;
	Color color;
};

struct death_anim {
	Vector2 position;
	int frameCounter = 0;
};

struct ShopItem {
	Texture2D tex;
	int index;
	Vector2 pos;
	int cost;
};

ShopItem items[3];

//bullet vectors
std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

//enemy vectors
std::vector<Enemy> enemy_tracker{};
std::vector<Enemy> enemy_pool{};

//obstacle vector
std::vector<Obstacle> obstacle_tracker{};
std::vector<Obstacle> obstacle_pool{};

//power up vector

std::vector<powerUp> powerUp_tracker{};
std::vector<powerUp> powerUp_pool{};

//death animations
std::vector<death_anim> deathAnim_tracker{};
std::vector<death_anim> deathAnim_pool{};

std::vector<Map> map_list{};

//TEXTURES
//Create gloval varaibles for all textures so they can be used by the draw function and the load assets function

Texture2D title_screen;
Texture2D win_screen;

//player character
Texture2D player_character_spritesheet;
Texture2D player_character_death;
Texture2D bullet_player;

//enemies
Texture2D orc_spritesheet;
Texture2D ogre_spritesheet;
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
Texture2D damaged_ogre;
Texture2D damaged_spyke_ball;
Texture2D death_animation_dungeon;
Texture2D death_animation_enemy;
Texture2D flyer_death_animation;

//shop
Texture2D ammunition;
Texture2D guns;
Texture2D boots;
Texture2D shop_keeper_spritesheet;
Texture2D shop_blanket;

//general terrain
Texture2D bridge;

//desert terrain
Texture2D dirt_grass;
Texture2D dirt;
Texture2D dirt_stones;
Texture2D path;
Texture2D path_stones;
Texture2D river_desert1, river_desert2;
Texture2D bush_spritesheet;
Texture2D logs;

//forest terrain
Texture2D grass;
Texture2D flowers_grass;
Texture2D path_grass;
Texture2D path_grass2;
Texture2D tall_grass;
Texture2D green_bush_spritesheet;
Texture2D stump;
Texture2D river_forest1;
Texture2D river_forest2;

//cementery terrain
Texture2D cementery_floor;
Texture2D cementery_gravel;
Texture2D cementery_path;
Texture2D stone_floor;
Texture2D tombstone;

//power ups
Texture2D coffee;
Texture2D coin;
Texture2D coin_5;
Texture2D extra_life;

//UI
Texture2D timer;
Texture2D power_up_slot;

//SOUND & MUSIC
//fx
Sound shoot_fx;
Sound enemy_death;
Sound power_up_pick_up;
Sound coin_sound;
Sound player_death;

//music
Music main_theme;

//DATA MANAGEMENT
std::vector<Texture2D> desert_textures{};
std::vector<Texture2D> forest_textures{};
std::vector<Texture2D> cementery_textures{};

void LoadAssets() {
	map_list = { Map("AREAS/area1_1.txt", 'D'), Map("AREAS/area1_2.txt", 'D'), Map("AREAS/area1_3.txt", 'D'), Map("AREAS/area1_4.txt", 'D'), Map("AREAS/area1_5.txt", 'D'), Map("AREAS/area2_1.txt", 'F'), Map("AREAS/area2_2.txt" , 'F'), Map("AREAS/area2_3.txt" , 'F'), Map("AREAS/area1_5.txt", 'F'), Map("AREAS/area3_1.txt", 'C')};
	// MAP TEXTURES
	//dessert
	dirt_grass = LoadTexture("dirt_grass.png");
	dirt = LoadTexture("dirt.png");
	dirt_stones = LoadTexture("dirt_stones.png");
	bush_spritesheet = LoadTexture("bush_spritesheet.png");
	path = LoadTexture("path.png");
	path_stones = LoadTexture("path_stones.png");
	logs = LoadTexture("logs.png");
	river_desert1 = LoadTexture("river_desert1.png");
	river_desert2 = LoadTexture("river_desert2.png");
	bridge = LoadTexture("bridge.png");

	//forest
	grass = LoadTexture("grass.png");
	flowers_grass = LoadTexture("flowers_grass.png");
	green_bush_spritesheet = LoadTexture("green_bush_spritesheet.png");
	path_grass = LoadTexture("path_grass.png");
	path_grass2 = LoadTexture("path_grass2.png");
	tall_grass = LoadTexture("tall_grass.png");
	stump = LoadTexture("stump.png");
	river_forest1 = LoadTexture("river_forest1.png");
	river_forest2 = LoadTexture("river_forest2.png");

	//cementery
	cementery_floor = LoadTexture("cementery_floor.png");
	cementery_gravel = LoadTexture("cementery_gravel.png");
	cementery_path = LoadTexture("cementery_path.png");
	stone_floor = LoadTexture("stone_floor.png");
	tombstone = LoadTexture("tombstone.png");


	//CHARACTERS
	player_character_spritesheet = LoadTexture("player_character_spritesheet.png");
	player_character_death = LoadTexture("player_character_death_anim.png");
	bullet_player = LoadTexture("bullet.png");

	//ENEMIES
	orc_spritesheet = LoadTexture("orc_spritesheet.png");
	ogre_spritesheet = LoadTexture("ogre_spritesheet.png");
	mushroom_spritesheet = LoadTexture("mushroom_spritesheet.png");
	cowboy_spritesheet = LoadTexture("cowboy_spritesheet.png");

	//DAMAGE & DEATH ANIMATIONS
	death_animation_enemy = LoadTexture("death_animation_enemy.png");

	//POWER UPS
	coin = LoadTexture("coin.png");
	extra_life = LoadTexture("extra_life.png");
	coin_5 = LoadTexture("coin_5.png");
	coffee = LoadTexture("coffe.png");

	//UI
	timer = LoadTexture("clock.png");
	power_up_slot = LoadTexture("power_up_slot.png");
	title_screen = LoadTexture("title_screen.png");
	win_screen = LoadTexture("end_title.png");

	//SHOP
	ammunition = LoadTexture("ammunition.png");
	boots = LoadTexture("boots.png");
	guns = LoadTexture("guns.png");
	shop_keeper_spritesheet = LoadTexture("shop_keeper_spritesheet.png");
	shop_blanket = LoadTexture("shop_blanket.png");

	//SOUND
	shoot_fx = LoadSound("shoot1.mp3");
	enemy_death = LoadSound("enemy_death2.mp3");
	power_up_pick_up = LoadSound("power_up_pick_up.mp3");
	coin_sound = LoadSound("coin.mp3");
	player_death = LoadSound("death_player.mp3");

	//MUSIC
	main_theme = LoadMusicStream("JOTPK_song.wav");

	desert_textures = { dirt, dirt_grass, dirt_stones, path_stones, path, bush_spritesheet, logs, river_desert1, river_desert2, bridge };
	forest_textures = { grass, flowers_grass, tall_grass, path_grass, path_grass2, green_bush_spritesheet, stump, river_forest1, river_forest2, bridge };
	cementery_textures = { cementery_floor, cementery_gravel, stone_floor, cementery_path, tombstone };
}

void UnloadGame() {
	//textures
	//dessert
	UnloadTexture(dirt_grass);
	UnloadTexture(dirt);
	UnloadTexture(bush_spritesheet);
	UnloadTexture(path);
	UnloadTexture(path_stones);
	UnloadTexture(logs);
	UnloadTexture(river_desert1);
	UnloadTexture(river_desert2);
	//forest
	UnloadTexture(grass);
	UnloadTexture(flowers_grass);
	UnloadTexture(path_grass);
	UnloadTexture(path_grass2);
	UnloadTexture(green_bush_spritesheet);
	UnloadTexture(tall_grass);
	UnloadTexture(stump);
	UnloadTexture(river_forest1);
	UnloadTexture(river_forest2);

	//win screen
	UnloadTexture(win_screen);

	//characters
	UnloadTexture(player_character_spritesheet);

	//enemies
	UnloadTexture(orc_spritesheet);

	//damage and death animations
	UnloadTexture(death_animation_enemy);

	//power ups
	UnloadTexture(coin);
	UnloadTexture(extra_life);
	UnloadTexture(coffee);
	UnloadTexture(coin_5);

	//shop
	UnloadTexture(shop_keeper_spritesheet);
	UnloadTexture(shop_blanket);
	UnloadTexture(boots);
	UnloadTexture(guns);
	UnloadTexture(ammunition);

	//sounds
	UnloadSound(shoot_fx);
	UnloadSound(enemy_death);
	UnloadSound(coin_sound);

	//music
	UnloadMusicStream(main_theme);

	CloseAudioDevice;
}

//INITAILIZE GAME

void InitGame() {
	SetTargetFPS(60);
	game_started = false;
	zoom_completed = false;
	lives = 3;
	if (level_count == 4 || level_count == 8) {
		player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, 7 * tile_size };
	}
	else {
		player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, top_margin + (area_size - player_size.y) / 2 + 16 };
	}
	InitAudioDevice();
	LoadAssets();
	//start playing music
	PlayMusicStream(main_theme);
}

void RestartGame() {
	InitGame();
}

void UpdateStartScreen() {
	/*static float zoom = 0.01f * (tile_size / 16);
	static bool zoom_completed = false;
	static float zoomSpeed = 0.005f * (tile_size / 16);*/

	if (!zoom_completed) {
		if (zoom < 1.0f * (tile_size / 16)) {
			zoom += zoom_speed;
		}
		else {
			zoom_completed = true;
		}
	}
	if (IsKeyDown(KEY_SPACE)) {
		game_started = true;
	}

}

void UpdateWinScreen() {
	if (IsKeyPressed(KEY_ENTER)) {
		close_game = true;
	}
}

void UpdateDeathScreen() {
	if (IsKeyPressed(KEY_ENTER)) {
		close_game = true;
	}
	/*if (IsKeyPressed(KEY_DOWN)) selected_option = 1;
	if (IsKeyPressed(KEY_UP)) selected_option = 0;
	if (IsKeyPressed(KEY_ENTER)) {
		if (selected_option == 0) RestartGame();
		else {
			close_game = true;
		}
	}*/

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

void createBoss() {
	bool bossExists = false;
	for (const auto& enemy : enemy_tracker) {
		if (enemy.isBoss) {
			bossExists = true;
			break;
		}
	}
	if (bossExists) return; // If a boss already exists, do not create another one

	Enemy boss;
	boss.position = { left_margin + (area_size - player_size.x) / 2 + 16,  area_size - 2 * tile_size }; // Set the boss position
	boss.type = 'B';
	boss.hp = 10;
	boss.speed = 2.0f;
	boss.isBoss = true; // Mark this enemy as a boss
	enemy_tracker.push_back(boss);
	active_enemies++;
}

void createEnemies() {
	if (level_count == 4 || level_count == 8) {
		if (!boss_defeated) {
			createBoss();
		}
		frames_since_enemy_spawn++;
		return;
	}
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
			pos = { left_margin, tile_size * 8 };
			break;
		case 4:
			pos = { left_margin, tile_size * 9 };
			break;
		case 5:
			pos = { left_margin, tile_size * 10 };
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
		default:
			cout << "the random value has a wrong value" << endl;
			break;
		}

		std::vector<char> allowedTypes = { 'O' };
		if (level_count == 2 || level_count == 3 || level_count == 5 || level_count == 6 || level_count == 7) {
			allowedTypes.push_back('G'); //add ogre to the allowed types
		}
		if (level_count == 5 || level_count == 6 || level_count == 7) {
			allowedTypes.push_back('M'); //add mushroom to the allowed types
		}
		int typeIdx = GetRandomValue(0, static_cast<int>(allowedTypes.size()) - 1);
		char enemyType = allowedTypes[typeIdx];

		if (enemy_pool.empty()) {
			Enemy baddie;
			baddie.position = pos;
			baddie.type = enemyType;
			switch (enemyType) {
			case 'O':
				baddie.hp = 1;
				baddie.speed = 1.25;
				break;
			case 'G':
				baddie.hp = 3;
				baddie.speed = 0.75;
				break;
			case 'M':
				baddie.hp = 2;
				baddie.speed = 1.5;
				break;
			default:
				cout << "the random value has a wrong value" << endl;
				break;
			}
			enemy_tracker.push_back(baddie);
		}
		else {
			enemy_tracker.push_back(enemy_pool.back());
			Enemy& e = enemy_tracker.back();
			e.type = enemyType;
			switch (enemyType) {
			case 'O':
				e.hp = 1;
				break;
			case 'G':
				e.hp = 3;
				break;
			case 'M':
				e.hp = 2;
				break;
			default: break;
			}
			e.position = pos;
			enemy_pool.pop_back();
		}
		active_enemies++;
	
	}
	else {
		frames_since_enemy_spawn++;
	}
}

bool checkEnemyObstacleCollision(Vector2 newPos, float size, const std::vector<Obstacle>& obstacles) {
	Rectangle enemyRec = { newPos.x, newPos.y, size, size };
	for (const auto& obstacle : obstacles) {
		if (CheckCollisionRecs(enemyRec, obstacle.get_rec())) {
			return true; // Collision detected
		}
	}
	return false; // No collision
}

void enemyMovement() {
	for (int i = 0; i < enemy_tracker.size(); i++) {
		Enemy& enemy = enemy_tracker[i];

		if (enemy.isBoss) {
			enemy.position.x += enemy.speed * enemy.boss_direction;
			if (enemy.position.x < enemy.boss_max_left) {
				enemy.position.x = enemy.boss_max_left;
				enemy.boss_direction = 1;
			}
			if (enemy.position.x > enemy.boss_max_right) {
				enemy.position.x = enemy.boss_max_right;
				enemy.boss_direction = -1;
			}

			// Boss bullet shooting logic
			if (boss_shoot_cooldown <= 0) {
				bullet b;
				b.damage = 1; // Boss bullet damage
				b.position = { enemy.position.x + tile_size / 2, enemy.position.y + tile_size / 2 };
				b.velocity = { 0, -1 }; // Boss shoots up
				b.speed = 3; // Boss bullet speed
				b.isBossBullet = true; // Mark as boss bullet
				bullet_tracker.push_back(b);
				boss_shoot_cooldown = 60; // Reset cooldown (1 second at 60 FPS)
			}
			else {
				boss_shoot_cooldown--;
			}

			continue;
		}

		float magnitude = sqrt(
			(player_pos.x - enemy.position.x) * (player_pos.x - enemy.position.x) +
			(player_pos.y - enemy.position.y) * (player_pos.y - enemy.position.y)
		);
		if (magnitude == 0) continue; // Avoid division by zero

		// Calculate intended movement
		float dx = ((player_pos.x - enemy.position.x) / magnitude) * enemy.speed;
		float dy = ((player_pos.y - enemy.position.y) / magnitude) * enemy.speed;

		// Try X movement
		Vector2 newPosX = { enemy.position.x + dx, enemy.position.y };
		if (!checkEnemyObstacleCollision(newPosX, tile_size, obstacle_tracker)) {
			enemy.position.x += dx;
		}

		// Try Y movement
		Vector2 newPosY = { enemy.position.x, enemy.position.y + dy };
		if (!checkEnemyObstacleCollision(newPosY, tile_size, obstacle_tracker)) {
			enemy.position.y += dy;
		}
	}
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
			b.position = { player_pos.x, player_pos.y};
			b.velocity = Shoot_dir;
			b.speed = 5;
			bullet_tracker.push_back(b);
		}
		else {
			bullet_tracker.push_back(bullet_pool.back());
			bullet_tracker.back().damage = damage;
			bullet_tracker.back().position = { player_pos.x + tile_size / 4 , player_pos.y + tile_size / 4 };
			bullet_tracker.back().velocity = Shoot_dir;
			bullet_tracker.back().speed = 5;
			bullet_tracker.back().isBossBullet = false; // Ensure it's not a boss bullet
			bullet_pool.pop_back();
		}
		fire_frame_counter++;
	}
	else if (fire_frame_counter % fire_rate != 0) {
		fire_frame_counter++;
	}
	//Shoot_dir = { 0, 0 };
}

void bulletUpdate() {
	//get the lenght of the bullet vector
	//update bullet's position
	for (int i = 0; i < bullet_tracker.size(); i++) {
		bullet& b = bullet_tracker[i];
		if (b.velocity.x != 0 && b.velocity.y != 0) {
			b.position = Vector2{ b.position.x + b.velocity.x * b.speed * 0.707f, b.position.y + b.velocity.y * b.speed * 0.707f};
		}
		else {
			b.position = Vector2{ b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed };
		}
	}

	//destroy bullets
	if (bullet_tracker.empty()) return;
	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
		//iterate and check all bullets if they are ouside of the map (should I check if they colisioned?, might only have to check the first shot if we don't check colisions)
		if ((bullet_tracker[i].position.x <= left_margin || bullet_tracker[i].position.x >= area_size + (left_margin) || bullet_tracker[i].position.y <= tile_size || bullet_tracker[i].position.y >= area_size + tile_size)) { //&& bullet_tracker[i] != NULL
			//save the bullet in the pool
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}

	}
}

//POWER UP
void powerUpUpdate() {
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
		powerUp_tracker[i].despawn_timer++;
		if (powerUp_tracker[i].despawn_timer > powerUp_lifespan) {
			//guardar power up al pool
			powerUp_pool.push_back(powerUp_tracker[i]);
			//borrar powerUp
			auto& k = powerUp_tracker.begin() + i;
			powerUp_tracker.erase(k);
		}
	}
}

void playerDeath() {
	player_dying = true;
	player_death_anim = 0;
	PlaySound(player_death);
	//borrar enemics
	for (int i = enemy_tracker.size() - 1; i >= 0; i--) {
		if (enemy_tracker[i].isBoss) {
			continue;
		}
		//save the bullet in the pool
		enemy_pool.push_back(enemy_tracker[i]);
		//borrar bullet
		auto& j = enemy_tracker.begin() + i;
		enemy_tracker.erase(j);

	}
	active_enemies = 0;
	//borrar totes les bales
	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
		//save the bullet in the pool
		bullet_pool.push_back(bullet_tracker[i]);
		//borrar bullet
		auto& j = bullet_tracker.begin() + i;
		bullet_tracker.erase(j);

	}
	//borrar tots els power ups
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
		//save the bullet in the pool
		powerUp_pool.push_back(powerUp_tracker[i]);
		//borrar bullet
		auto& j = powerUp_tracker.begin() + i;
		powerUp_tracker.erase(j);

	}
}

void bossBullet_playerColl() {
	if (bullet_tracker.empty()) return;

	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
		if (bullet_tracker[i].isBossBullet) {
			// Check collision with player
			if (CheckCollisionCircles(
				{ bullet_tracker[i].position.x + tile_size / 2, bullet_tracker[i].position.y + tile_size / 2 }, tile_size / 8,
				{ player_pos.x + player_size.x / 2, player_pos.y + player_size.y / 2 }, player_size.x / 2)) {

				// Player takes damage or dies
				lives--;
				playerDeath();

				// Optionally, break if you want only one bullet to hit per frame
				// break;
				return;
			}
		}
	}
}

//COLLISIONS
void player_enemyColl() {
	bool already_damaged = false;
	int i = 0;
	while (!already_damaged && i < enemy_tracker.size()) {
		if (CheckCollisionCircles({ player_pos.x + (player_size.x / 2), player_pos.y + (player_size.y / 2) }, player_size.x / 2, { enemy_tracker[i].position.x + (tile_size/2), enemy_tracker[i].position.y + (tile_size/2)}, tile_size / 2)) {
			already_damaged = true;
			lives--;

			//player death
			playerDeath();

			if (lives < 0) {
				cout << "you are dead" << endl;
			}
		}
		i++;
	}
}

float GetAngle(Vector2 v, Vector2 u) {
	float c = (u.x * v.x + u.y * v.y); //prop al cosinus
	float s = (u.x * v.y - u.y * v.x); //prop al sinus

	return atan2(s, c) + PI;
}


void restrainPlayerMovement(const Rectangle& obstacle_rec) {
	Rectangle player_rec = { player_pos.x, player_pos.y, player_size.x, player_size.y };

	// Calculate overlap on both axes
	float overlapLeft = (player_rec.x + player_rec.width) - obstacle_rec.x;
	float overlapRight = (obstacle_rec.x + obstacle_rec.width) - player_rec.x;
	float overlapTop = (player_rec.y + player_rec.height) - obstacle_rec.y;
	float overlapBottom = (obstacle_rec.y + obstacle_rec.height) - player_rec.y;

	// Find the axis of least penetration
	float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
	float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

	if (minOverlapX < minOverlapY) {
		// Resolve on X axis
		if (overlapLeft < overlapRight) {
			player_pos.x -= overlapLeft;
			xPosBlock = true;
		}
		else {
			player_pos.x += overlapRight;
			xNegBlock = true;
		}
	}
	else {
		// Resolve on Y axis
		if (overlapTop < overlapBottom) {
			player_pos.y -= overlapTop;
			yPosBlock = true;
		}
		else {
			player_pos.y += overlapBottom;
			yNegBlock = true;
		}
	}
}

void player_obstacleColl() {
	Rectangle rec_player = { player_pos.x, player_pos.y, tile_size, tile_size };
	for (int i = 0; i < obstacle_tracker.size(); i++) {
		Rectangle rec_obstacle = obstacle_tracker[i].get_rec();
		if (CheckCollisionRecs(rec_player, rec_obstacle)) {
			restrainPlayerMovement(rec_obstacle);
		}
	}
}

void spawnPowerUp(float x, float y) {
	randVal = GetRandomValue(1, 5);
	if (randVal == 1) {
		char C;
		randVal = GetRandomValue(0, 3);
		switch (randVal) {
		case 0: //extra life
			C = 'U';
			break;
		case 1: //coin
			C = 'O';
			break;
		case 2: //coin 5
			C = 'T';
			break;
		case 3: //coffee
			C = 'D';
			break;
		default:
			cout << "type of power up not recognised" << endl;
			C = '.';
			break;

		}
		
		if (powerUp_pool.empty()) {
			//en crea un
			powerUp p;
			p.position = { x, y };
			p.type = C;
			powerUp_tracker.push_back(p);
		}
		else {
			// agafa un del pool
			powerUp_tracker.push_back(powerUp_pool.back());
			powerUp_tracker.back().position = {x , y};
			powerUp_tracker.back().type = C;
			powerUp_tracker.back().despawn_timer = 0;
			powerUp_pool.pop_back();
		}
		
	}
}

void createDeathAnimation(Vector2 pos) {
		PlaySound(enemy_death);
		if (deathAnim_pool.empty()) {
			death_anim d;
			d.position = pos;
			deathAnim_tracker.push_back(d);
		}
		else {
			deathAnim_tracker.push_back(deathAnim_pool.back());
			deathAnim_tracker.back().position = pos;
			deathAnim_tracker.back().frameCounter = 0;
			deathAnim_pool.pop_back();
		}
}

void bullet_enemyColl() { //bug here?
	//comprovar totes les bales per tots els enemics
	for (int i = enemy_tracker.size() - 1; i >= 0; i--) {
		for (int j = bullet_tracker.size() - 1; j >= 0; j--) {
			if (enemy_tracker.size() - 1 >= i && bullet_tracker.size() - 1 >= j && !enemy_tracker.empty() && !bullet_tracker.empty()) {
				if (CheckCollisionCircles({ bullet_tracker[j].position.x + tile_size / 2, bullet_tracker[j].position.y + tile_size / 2 }, tile_size / 8, { enemy_tracker[i].position.x + tile_size/2, enemy_tracker[i].position.y + tile_size / 2 }, tile_size / 2)) {
					if (enemy_tracker[i].isBoss && bullet_tracker[j].isBossBullet) {
						// Boss bullet hits the boss, no damage
						continue;
					}
					//save the bullet in the pool
					auto bullet = bullet_tracker[j];
					bullet_pool.push_back(bullet);
					//borrar bullet
					bullet_tracker.erase(bullet_tracker.begin() + j);


					//reduce hit enemy hitpoints
					enemy_tracker[i].hp -= damage;
				
					//kill enemy if hitpoints are 0 or lower
					if (enemy_tracker[i].hp <= 0) {

						//save the enemy in the pool
						if (!enemy_tracker[i].isBoss) {
							enemy_pool.push_back(enemy_tracker[i]);
						}

						//mirar si es crea un power up
						spawnPowerUp(enemy_tracker[i].position.x, enemy_tracker[i].position.y);

						//crea un death animations object
						createDeathAnimation(enemy_tracker[i].position);

						if (enemy_tracker[i].isBoss) {
							boss_defeated = true;
						}

						//borrar enemy
						auto& e = enemy_tracker.begin() + i;
						enemy_tracker.erase(e);
						active_enemies--;

					}
				}
			}
			
		}
	}
}

void bullet_obstacleColl() { //bug here?
	for (int j = 0; j < obstacle_tracker.size(); j++) {
		for (int i = bullet_tracker.size() - 1; i >= 0; i--) {

			if (obstacle_tracker[j].get_type() == 'R' || obstacle_tracker[j].get_type() == 'T') {
				//if the obstacle is a river or a wall, we don't check for collisions
				continue;
			}

			else if (CheckCollisionCircles({ bullet_tracker[i].position.x + tile_size/2, bullet_tracker[i].position.y + tile_size / 2 }, tile_size / 8, { obstacle_tracker[j].get_rec().x + tile_size / 2, obstacle_tracker[j].get_rec().y + tile_size / 2}, tile_size / 2)) {
				//save the bullet in the pool
				bullet_pool.push_back(bullet_tracker[i]);
				//borrar bullet
				auto& k = bullet_tracker.begin() + i;
				bullet_tracker.erase(k);
			}
		}
	}
}

void player_powerUpColl() {
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--){
		if (CheckCollisionCircles({ player_pos.x + (player_size.x / 2), player_pos.y + (player_size.y / 2) }, player_size.x / 2, { powerUp_tracker[i].position.x + tile_size / 2, powerUp_tracker[i].position.y + tile_size / 2 }, tile_size / 4)) {
			switch (powerUp_tracker[i].type) {
			case 'U': //vida extra
				lives++;
				PlaySound(power_up_pick_up);
				break;
			case 'O'://monedes
				coins++;
				PlaySound(coin_sound);
				break;
			case 'T'://monedes
				coins += 5;
				PlaySound(coin_sound);
				break;
			case 'D': //coffee
				PlaySound(power_up_pick_up); //same sound as picking up coffee
				if (stored_power_up == ' ') stored_power_up = 'D';
				else {
					// If slot already has something, auto-activate new one
					if (power_up_timer <= 0) {
						active_power_up_type = 'D';
						power_up_timer = 16 * 60;
						player_Speed += 0.5f;
						PlaySound(power_up_pick_up);//same sound as using power-up
					}
				}
				break;
			}
			//guardar power up al pool
			powerUp_pool.push_back(powerUp_tracker[i]);
			//borrar powerUp
			auto& k = powerUp_tracker.begin() + i;
			powerUp_tracker.erase(k);
		}
	}
}

void runPowerUp() {
	// Activate stored power-up on spacebar
	if (IsKeyPressed(KEY_SPACE) && stored_power_up != ' ' && power_up_timer <= 0) {
		active_power_up_type = stored_power_up;
		stored_power_up = ' ';
		PlaySound(power_up_pick_up); //same sound as using power-up
		if (active_power_up_type == 'D') {
			player_Speed += 0.5f;
			power_up_timer = 16 * 60;
		}
	}
	// Tick active power-up
	if (power_up_timer > 0) {
		power_up_timer--;
		// Auto-end after timer or when level ends 
		if (power_up_timer <= 0 || (active_enemies == 0 && frames_since_level_start >= level_length) || shop_active) {
			if (active_power_up_type == 'D') player_Speed -= 0.5f;
			active_power_up_type = ' ';
			power_up_timer = 0;
		}
	}
}


void updateDeathAnimations() {
	//eliminar els objectes que porten 10 segons en joc
	for (int i = deathAnim_tracker.size() - 1; i >= 0; i--) {
		//eliminar els objectes que porten 10 segons en joc
		deathAnim_tracker[i].frameCounter++;

		if (deathAnim_tracker[i].frameCounter >= 60 * 10) {
			//save the death animation in the pool
			deathAnim_pool.push_back(deathAnim_tracker[i]);
			//borrar death animation
			auto& j = deathAnim_tracker.begin() + i;
			deathAnim_tracker.erase(j);
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
			player_right_foot = !player_right_foot;
			//play walking sound

		}
	}
	else {
		player_walk_anim_counter = 0;
	}

	if (player_dying) {
		player_death_anim++;
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

void changeLevel() {
	if (level_count == 4 || level_count == 8) {
		if (IsKeyDown('N') && boss_defeated) {
			shop_active = false;
			frames_since_level_start = 0;
			//delete all obstacles
			obstacle_tracker.clear();
			obstacles_positioned = false;
			powerUp_tracker.clear();
			deathAnim_tracker.clear();
			if (level_count + 1 < map_list.size()) {//adds one to the index of the map vector
				level_count++;
			}
			else {//repeats to the first level if player is on the last
				level_count = 0;
				game_won = true;
			}
			if (level_count == 4 || level_count == 8) {
				player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, tile_size * 7 };
			}
			else {
				player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, top_margin + (area_size - player_size.y) / 2 + 16 };
			}
			boss_defeated = false;
		}
	}
	else {
		if (IsKeyDown('N') && frames_since_level_start >= level_length && active_enemies == 0) {//checks if the timer is over and if there aren't any enemies in order to advance
			shop_active = false;
			frames_since_level_start = 0;
			//delete all obstacles
			obstacle_tracker.clear();
			obstacles_positioned = false;
			powerUp_tracker.clear();
			deathAnim_tracker.clear();
			bullet_tracker.clear();
			if (level_count + 1 < map_list.size()) {//adds one to the index of the map vector
				level_count++;
			}
			else {//repeats to the first level if player is on the last
				game_won = true;
				level_count = 0;
			}

			if (level_count == 4 || level_count == 8) {
				player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, tile_size * 7 };
			}
			else {
				player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, top_margin + (area_size - player_size.y) / 2 + 16 };
			}
			boss_defeated = false;
		}
	}
}
//SHOP

void StartShop() {
	shop_active = true;
	shop_item_bought = false;
	shop_phase = 1;
	shopkeeper_pos = { (area_size / 2) + left_margin, tile_size };
}

void DrawShopkeeper() {
	Vector2 shopkeeper_dest = { (area_size / 2) + left_margin, area_size };
	Rectangle shopkeeper_src = { 16 * shopkeeper_frame, 0, 16, 16 };
	DrawTexturePro(shop_keeper_spritesheet, shopkeeper_src,
		{ shopkeeper_pos.x, shopkeeper_pos.y, tile_size, tile_size },
		{ 0, 0 }, 0, WHITE);
}

int GetItemCost(int id) {
	if (level_count == 1) return (id == 0 ? 8 : id == 1 ? 10 : 15);
	if (level_count == 3) return (id == 0 ? 8 : id == 1 ? 20 : 15);
	if (level_count == 5) return (id == 0 ? 8 : id == 1 ? 20 : 30);
	if (level_count == 7) return (id == 0 ? 8 : id == 1 ? 20 : 45);
	return 999;
}

void DrawShopUI() {

	Vector2 ui_pos = { (area_size / 2) + left_margin, (area_size / 3) + tile_size };

	for (int i = 0; i < 3; i++) {
		Texture2D tex;
		int index = 0;

		switch (i) {
		case 0: // Boots
			tex = boots;
			index = 0;
			break;

		case 1: // Gun
			tex = guns;
			if (level_count <= 1) index = 0;
			else index = 1;
			break;

		case 2: // Backpack
			tex = ammunition;
			if (level_count < 5) index = 0;
			else if (level_count < 7) index = 1;
			else index = 2;
			break;
		}

		items[i] = {
			tex,
			index,
			{ ui_pos.x + tile_size * i * 1.25f, ui_pos.y + (tile_size / 4) },
			GetItemCost(i)
		};
	}

	if (!shop_item_bought) {

		Rectangle src = { 0, 0, 64, 32 };
		Rectangle dest = { ui_pos.x - tile_size, ui_pos.y, tile_size * 4, tile_size * 2 };
		DrawTexturePro(shop_blanket, src, dest, { 0, 0 }, 0.0f, WHITE);

		// Draw each item
		for (int i = 0; i < 3; i++) {
			Rectangle src = { 16.0f * items[i].index, 0, 16, 16 };
			Rectangle dst = { items[i].pos.x - tile_size / 1.4, items[i].pos.y - tile_size / 4, tile_size, tile_size };
			DrawTexturePro(items[i].tex, src, dst, { 0, 0 }, 0, WHITE);

			// Cost text
			DrawTextEx(font, TextFormat("%i", items[i].cost),
				{ items[i].pos.x - tile_size / 1.8f, items[i].pos.y + tile_size * 0.75f },
				10 * (tile_size / 16), 1, BLACK);

		}
	}
	if (item_being_held_up >= 0 && item_being_held_up < 3 && shop_phase == 3) {
		Texture2D tex = items[item_being_held_up].tex;
		int index = items[item_being_held_up].index;
		Rectangle src = { 16.0f * (index), 0, 16, 16 };
		Rectangle dest = { player_pos.x, player_pos.y - tile_size, tile_size, tile_size };
		DrawTexturePro(tex, src, dest, { 0, 0 }, 0.0f, WHITE);
	}

	if (items[item_being_held_up].cost == 8 && items[item_being_held_up].index == 0) { //boots
		boot_bought = true;
	}
	else if ((items[item_being_held_up].cost == 10 || items[item_being_held_up].cost == 20) && items[item_being_held_up].index <= 2) {
		if (items[item_being_held_up].index > gun_upgrade_level) gun_upgrade_level = items[item_being_held_up].index;
	}
	//this code is so dumb but i couldn't think of doing it another way
	else if ((items[item_being_held_up].cost == 15 || items[item_being_held_up].cost == 30 || items[item_being_held_up].cost == 45) && items[item_being_held_up].index >= 0) {
		if (items[item_being_held_up].index > backpack_upgrade_level) backpack_upgrade_level = items[item_being_held_up].index;
	}

}

bool CanBuyItem(int id) {
	if (id == 0) return !boot_bought; // boot
	if (id == 1) {
		if (level_count == 1) return gun_upgrade_level < 1;
		else return gun_upgrade_level < 2;
	}      // gun
	if (id == 2) {                            // backpack (per stage)
		int stage = (level_count >= 7 ? 2 : level_count >= 5 ? 1 : 0);
		return !backpack_bought[stage];
	}
	return false;
}

void ApplyItemEffect(int id) {
	switch (id) {
	case 0: // Boots
		player_Speed += 0.5f;
		boot_bought = true;
		break;

	case 1: // Gun
		if (level_count == 1 && gun_upgrade_level < 1) {
			fire_rate -= 2;
			gun_upgrade_level = 1;
		}
		else if (level_count > 1 && gun_upgrade_level < 2) {
			fire_rate -= 1;
			gun_upgrade_level = 2;
		}
		break;

	case 2: // Backpack/ammunition
		int stage = (level_count >= 7 ? 2 : level_count >= 5 ? 1 : 0);
		if (backpack_upgrade_level < stage + 1) {
			damage += 1;
			backpack_upgrade_level = stage + 1;
		}
		break;
	}
}


void HandleShopPurchase() {
	Vector2 ui_pos = { (area_size / 2) + left_margin - tile_size * 2, area_size - tile_size * 4 };
	for (int i = 0; i < 3; i++) {

		Vector2 item_center = {
			items[i].pos.x + (tile_size / 4) - tile_size / 1.4,
			items[i].pos.y + (tile_size / 2) - tile_size / 4
		};

		if (CheckCollisionCircles(
			{ player_pos.x + (player_size.x / 2), player_pos.y + (player_size.y / 2) },
			player_size.x / 2,
			{ item_center.x + (tile_size / 2), item_center.y + (tile_size / 2) },
			tile_size / 4)) {

			int cost = GetItemCost(i);
			if (coins >= cost && CanBuyItem(i)) {
				coins -= cost;
				ApplyItemEffect(i);
				item_being_held_up = i;
				if (i == 1) bought_gun_sprite = items[i].index;
				if (i == 2) bought_backpack_sprite = items[i].index;
				shop_item_bought = true;
				break;
			}
		}
	}
}

void UpdateShop() {
	float speed = area_size / 600.0f; // ~2 seconds to center
	float shop_timer = 0; // for timing entry/exit animations
	if (shop_phase == 0 && shop_active && last_shop_level == level_count) {
		// prevent repeat on same level
		shop_active = false;
		return;
	}
	if (shop_phase == 1) { // entering
		shopkeeper_pos.y += speed;
		shop_timer += GetFrameTime();
		if ((int)(shopkeeper_pos.y / tile_size * 3) % 2 == 0) shopkeeper_frame = 0;
		else shopkeeper_frame = 1;
		if (shopkeeper_pos.y >= area_size / 3) {
			shopkeeper_pos.y = area_size / 3;
			shop_phase = 2;
			last_shop_level = level_count;
		}
	}
	else if (shop_phase == 2) { // idle/shop active
		Vector2 dir_to_player = {
		(player_pos.x + player_size.x / 2) - (shopkeeper_pos.x + tile_size / 2),
		(player_pos.y + player_size.y / 2) - (shopkeeper_pos.y + tile_size / 2)
		};
		// Use atan2 to calculate angle if needed, but simple x-direction works fine here:
		if (fabsf(dir_to_player.x) > tile_size * 0.75f) { // more than ~45 degrees horizontally
			shopkeeper_frame = (dir_to_player.x > 0) ? 4 : 3;
		}
		else {
			shopkeeper_frame = 2; // center/idle
		}
		HandleShopPurchase(); // allows item collision
		if (shop_item_bought) {
			shop_phase = 3;
			shop_timer = 0;
			hold_up_timer = 2.0f;
		}
	}
	else if (shop_phase == 3) { // hold-up + exit
		shop_timer += GetFrameTime();
		hold_up_timer -= GetFrameTime();
		shopkeeper_frame = 0;
		if (hold_up_timer <= 0) {
			item_being_held_up = -1;
			shopkeeper_pos.y -= speed;
			if ((int)(shopkeeper_pos.y / tile_size * 3) % 2 == 0) shopkeeper_frame = 0;
			else shopkeeper_frame = 1;
			if (shopkeeper_pos.y <= tile_size) {
				shop_active = false;
				shop_phase = 0;
			}
		}
	}


}

//UPDATE GAME
void UpdateGame() {//update variables and positions

	if (game_started && lives >= 0) {
		//MUSIC
		UpdateMusicStream(main_theme);
		if (!player_dying) {
			//MOVEMENT
			PlayerMovement();
			//ENEMY MOVEMENT
			enemyMovement();

			//CREATE ENEMIES
			if (frames_since_level_start < level_length) {
				createEnemies();
				//incrementar variable temps
				frames_since_level_start++;
			}
			//SHOOTING BULLETS
			bulletShooting();

			//update bullets
			bulletUpdate();
			bossBullet_playerColl();

			//update power ups
			powerUpUpdate();

			//COLLISIONS
			//player-enemies
			player_enemyColl();

			//player-obstacles
			player_obstacleColl();

			//bullet-enemies
			bullet_enemyColl();

			//bullets-obstacles
			bullet_obstacleColl();

			//player power-up colisions
			player_powerUpColl();

			runPowerUp();

			if (frames_since_level_start >= level_length && active_enemies <= 0 && !shop_active &&
				last_shop_level != level_count &&
				(level_count == 1 || level_count == 3 || level_count == 5 || level_count == 7)) {
				StartShop();
			}


			if (shop_active) {
				DrawShopkeeper();
				DrawShopUI();
				UpdateShop();
			}

			if (shop_phase != 3) {
				changeLevel();
			}
		}

		//update death animations
		updateDeathAnimations();

		//ANIMATIONS
		animationManager();
	}
	else if (!game_started) {//start of the game
		UpdateStartScreen();
	}
	else {//death of the player
		UpdateDeathScreen();
	}

	
}

//DRAW
void DrawPlayer() {
	//shop animation
	if (shop_phase == 3 && item_being_held_up != -1) {
		if (player_Speed != 0) saved_speed = player_Speed;
		player_Speed = 0;
		Rectangle holding_src = { 16, 0, 16, 16 };
		Rectangle holding_dest = { player_pos.x, player_pos.y, player_size.x, player_size.y };
		DrawTexturePro(player_character_spritesheet, holding_src, holding_dest, { 0, 0 }, 0, WHITE);
		return;
	}
	else if (player_Speed == 0 && saved_speed > 0) {
		player_Speed = saved_speed;
		saved_speed = 0;
	}
	if (anim_dir != 0) {//esta disparant
		if (Mov_dir != 0) { //s'esta movent
			switch (anim_dir) {
			case 1: //dreta
				if (player_right_foot) {
					src = { 16 * 6, 0, 16, 16 };
				}
				else {
					src = { 0, 16, 16, 16 };
				}
				break;
			case 2: //avall
				if (player_right_foot) {
					src = { 16 * 4, 0, 16, 16 };
				}
				else {
					src = { 16 * 3, 0, 16, 16 };
				}
				break;
			case 3: //esquerra
				if (player_right_foot) {
					src = { 16 * 6, 16, 16, 16 };
				}
				else {
					src = { 16 * 5, 16, 16, 16 };
				}
				break;
			case 4: //amunt 
				if (player_right_foot) {
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
			if(player_right_foot){
				src = { 16 * 6, 0, 16, 16 };
			}
			else {
				src = { 0, 16, 16, 16 };
			}
			break;
		case 2: //avall
			if (player_right_foot) {
				src = {16 * 4, 0, 16, 16};
			}
			else {
				src = { 16 * 3, 0, 16, 16};
			}
			break;
		case 3: //esquerra
			if (player_right_foot) {
				src = {16 * 6, 16, 16, 16};
			}
			else {
				src = { 16 * 5, 16, 16, 16};
			}
			break;
		case 4: //amunt 
			if (player_right_foot) {
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
void DrawPlayerDeath() {
	//player_walk_anim_counter++;
	//actualitzar animacions
	if (player_death_anim < 10) {
		src = { 0, 0, 16, 16 };
	}
	else if (player_death_anim < 10 * 2) {
		src = { 16 * 1, 0, 16, 16 };
	}
	else if (player_death_anim < 10 * 3) {
		src = { 16 * 2, 0, 16, 16 };
	}
	else if (player_death_anim < 10 * 4) {
		src = { 16 * 3, 0, 16, 16 };
	}
	else if (player_death_anim <= 10 * 10) {}
	else {
		player_dying = false;
		if (level_count == 4 || level_count == 8) {
			player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, tile_size * 7 };
		}
		else {
			player_pos = { left_margin + (area_size - player_size.x) / 2 + 16, top_margin + (area_size - player_size.y) / 2 + 16 };
		}
		player_death_anim = 0;
		player_walk_anim_counter = 0;
		player_death_anim = 0;
	}
	//dibuixa si han passat menys de 40 frames 
	if (player_death_anim < 10 * 4) {
		//waits
		DrawTexturePro(player_character_death, src, { player_pos.x, player_pos.y, tile_size, tile_size }, { 0,0 }, 0, WHITE);
	}
	//DrawTexturePro(player_character_death, src, { player_pos.x, player_pos.y, tile_size, tile_size }, { 0,0 }, 0, WHITE);
}

void DrawUI() {
	//draw clock
	DrawTextureEx(timer, { static_cast<float>(left_margin + 10), static_cast<float>(tile_size * 0.45f / 2) }, 0, tile_size / 16, WHITE);

	//draw life 
	DrawTextureEx(extra_life, { 10, left_margin + 20 }, 0, (tile_size / 16) * 1.25, WHITE);
	//write actual lives
	DrawText(TextFormat("X%i", lives), tile_size + tile_size / 2 + 10, left_margin + tile_size / 4 + 20, 10 * (tile_size / 16), WHITE);

	//draw coins
	DrawTextureEx(coin, { 10, tile_size * 4 + 20 }, 0, (tile_size / 16) * 1.25, WHITE);
	//write actual coins
	DrawText(TextFormat("X%i", coins), tile_size + tile_size/2 + 10, tile_size * 4 + tile_size / 4 + 20, 10 * (tile_size / 16), WHITE);
	
	//draw power up slot
	DrawTextureEx(power_up_slot, { tile_size / 2 + 5, tile_size / 2 + 25}, 0, (tile_size / 16) * 1.25, WHITE);
	
	//power-ups
	if (stored_power_up == 'D') {
		DrawTextureEx(coffee, { tile_size * 0.9f, tile_size * 1.5f }, 0, (tile_size / 16) * 1.25, WHITE);
	}

	//draw upgrades in bottom corner
	if (boot_bought) {
		Rectangle src = { 0, 0, 16, 16 }; // Boot image never changes
		DrawTexturePro(boots, src, { left_margin - tile_size, area_size - (tile_size * 2), tile_size, tile_size }, { 0, 0 }, 0, WHITE);
	}

	// 2. Gun (if bought)
	if (gun_upgrade_level >= 0) {
		int index = bought_gun_sprite;
		Rectangle src = { 16.0f * (index), 0, 16, 16 };
		DrawTexturePro(guns, src, { left_margin - tile_size, area_size - tile_size, tile_size , tile_size }, { 0, 0 }, 0, WHITE);
	}

	// 3. Backpack (if any ammo level bought)
	if (backpack_upgrade_level >= 0) {
		int index = bought_backpack_sprite;
		Rectangle src = { 16.0f * (index), 0, 16, 16 };
		DrawTexturePro(ammunition, src, { left_margin - tile_size, area_size, tile_size  , tile_size }, { 0, 0 }, 0, WHITE);
	}

	if (level_count == 4 || level_count == 8) {
		int bossCurrentHP = 0;
		for (const auto& enemy : enemy_tracker) {
			if (enemy.isBoss) {
				bossCurrentHP = enemy.hp;
				break;
			}
		}
		const int BOSS_MAX_HP = 10;
		float barMaxWidth = area_size;
		float barHeight = tile_size * 0.4f;
		float barX = left_margin;
		float barY = area_size + tile_size - barHeight;
		float barWidth = (bossCurrentHP / (float)BOSS_MAX_HP) * barMaxWidth;
		if (bossCurrentHP > 0) {
			DrawRectangle(barX, barY, barWidth, barHeight, RED);
			DrawRectangleLines(barX, barY, barMaxWidth, barHeight, BLACK);
		}
	}
	else {
		int length = area_size - (area_size * (frames_since_level_start / level_length));
		DrawRectangle(tile_size * 4, tile_size * 0.45, length, tile_size * 0.4, GREEN);
	}
}
void DrawEnemies() {
	int enemy_amount = enemy_tracker.size();
	for (int i = 0; i < enemy_amount; i++) {
		Texture2D texture;
		if (enemy_tracker[i].isBoss) {
			texture = cowboy_spritesheet;
			if (enemy_tracker[i].right_foot) {
				src = { 32, 0, 16, 16 };
			}
			else {
				src = { 48, 0, 16, 16 };
			}
		}
		else {
			switch (enemy_tracker[i].type) {
			case 'O': //orc
				texture = orc_spritesheet;
				break;
			case 'G': // ogre
				texture = ogre_spritesheet;
				break;
			case 'M': // mushroom
				texture = mushroom_spritesheet;
				break;
			default:
				cout << "enemy type not recognized" << endl;
				texture = orc_spritesheet; //default texture
				break;
			}
			if (enemy_tracker[i].right_foot) {
				src = { 0, 0, 16, 16 };
			}
			else {
				src = { 16, 0, 16, 16 };
			}
		}
		
		DrawTexturePro(texture, src, { enemy_tracker[i].position.x, enemy_tracker[i].position.y , tile_size, tile_size }, { 0,0 }, 0, WHITE);
	}
}
void DrawBullets() {
	int bullet_amount = bullet_tracker.size();
	for (int i = 0; i < bullet_amount; i++) {
		DrawTextureEx(bullet_player, bullet_tracker[i].position, 0, tile_size / 16, WHITE);
	}
}
void DrawDeathAnimations() {
	for (int i = 0; i < deathAnim_tracker.size(); i++) {
		//actualitzar animacions
		if (deathAnim_tracker[i].frameCounter < 8) {
			src = { 0, 0, 16, 16 };
		}
		else if (deathAnim_tracker[i].frameCounter < 8 * 2) {
			src = { 16 * 1, 0, 16, 16 };
		}
		else if (deathAnim_tracker[i].frameCounter < 8 * 3) {
			src = { 16 * 2, 0, 16, 16 };
		}
		else if (deathAnim_tracker[i].frameCounter < 8 * 4) {
			src = { 16 * 3, 0, 16, 16 };
		}
		else if (deathAnim_tracker[i].frameCounter < 8 * 5) {
			src = { 16 * 4, 0, 16, 16 };
		}
		else {
			src = { 16 * 5, 0, 16, 16 };
		}
		DrawTexturePro(death_animation_enemy, src, {deathAnim_tracker[i].position.x, deathAnim_tracker[i].position.y, tile_size, tile_size}, { 0,0 }, 0, WHITE);
	}
}
void drawPowerUps() {
	for (int i = 0; i < powerUp_tracker.size(); i++) {
		switch (powerUp_tracker[i].type) {
		case 'U':
			DrawTextureEx(extra_life, { powerUp_tracker[i].position }, 0, tile_size / 16, WHITE);
			break;
		case 'O':
			DrawTextureEx(coin, { powerUp_tracker[i].position }, 0, tile_size / 16, WHITE);
			break;
		case 'T':
			DrawTextureEx(coin_5, { powerUp_tracker[i].position }, 0, tile_size / 16, WHITE);
			break;
		case 'D':
			DrawTextureEx(coffee, { powerUp_tracker[i].position }, 0, tile_size / 16, WHITE);
			break;
		default:
			cout << "power up not recognized (drawing)" << endl;
			break;
		}
	}
}


void DrawStartScreen() {
	ClearBackground(BLACK);

	//DrawTextureEx(titleScreen, { (area_size/2) - ((tile_size) * 6), (area_size / 2) - ((tile_size) * 5) }, 0, zoom, WHITE);

	//DrawTextureEx(title_screen, { ((area_size + (tile_size * 3)) / 2) - ((tile_size) * 3), ((area_size + tile_size) / 2) - ((tile_size) * 3) }, 0, tile_size/16, BLACK);

	DrawTexturePro(
		title_screen,
		{ 0, 0, 16 * 6, 16 * 5 },
	{
		left_margin + (area_size - player_size.x) / 2 - 40 - (tile_size * 6 * zoom) / 2,
		top_margin + (area_size - player_size.y) / 2 - (tile_size * 5 * zoom) / 2,
		tile_size * 6 * zoom,
		tile_size * 5 * zoom
	},
		{ 0, 0 },
		0,
		WHITE
	);
}
void DrawWinScreen() {
	ClearBackground(BLACK);

	// Draw your win screen image centered on the screen
	float x = (area_size - tile_size) / 2.0f;
	float y = (area_size - (tile_size * 4)) / 2.0f;
	DrawTextureEx(win_screen, { x, y }, 0, 2, WHITE);

	// Optional: draw instruction text
	DrawText("Press ENTER to exit", (area_size + left_margin) / 2 - (tile_size * 3), area_size - tile_size, 20, WHITE);
}

void DrawGameOverScreen() {
	ClearBackground(BLACK);
	if (!close_game) {
		DrawText("Game Over", area_size / 2 - 80, area_size / 2 - 40, 40, WHITE);

		DrawText("Quit", area_size / 2 - 80, area_size / 2 + 80, 20, RED);

		//DrawText("Restart", area_size / 2 - 80, area_size / 2 + 40, 20, (selected_option == 0) ? RED : WHITE);
		//DrawText("Quit", area_size / 2 - 80, area_size / 2 + 80, 20, (selected_option == 1) ? RED : WHITE);
		//Vector2 arrowPos = { area_size / 2 - 70, area_size / 2 + 40 + (selected_option * 40) };
		//DrawTriangleLines(arrowPos, { arrowPos.x + 10, arrowPos.y - 5 }, { arrowPos.x + 10, arrowPos.y + 5 }, WHITE);
	}
}
void DrawGame() {//draws the game every frame
	BeginDrawing();
	if (game_won) {
		UpdateWinScreen();
		DrawWinScreen();
	} else if (game_started && lives >= 0) {
		//draw background
		ClearBackground(BLACK);

		//draw map
		map_list[level_count].draw(tile_size, left_margin, bush_frame, obstacles_positioned, obstacle_pool, obstacle_tracker, desert_textures, forest_textures, cementery_textures);

		//draw enmie death animation
		DrawDeathAnimations();

		//draw UI
		DrawUI();

		//shop
		if (shop_active) {
			DrawShopkeeper(); // always draw the shopkeeper if shop is active
			if (shop_phase == 2 || shop_phase == 3) {
				DrawShopUI(); // only draw shop UI after shopkeeper animation
			}
		}

		//draw player
		if (!player_dying) {
			DrawPlayer();
		}
		else {
			DrawPlayerDeath();
		}

		//draw enemies
		DrawEnemies();

		//draw power ups
		drawPowerUps();

		//draw bullets
		DrawBullets();

	}
	else if (!game_started) {
		DrawStartScreen();
	}
	else {
		DrawGameOverScreen();
	}
	EndDrawing();
}


void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}



int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(area_size + left_margin, area_size + tile_size, "Journey of the prairie king");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");


	//start some values
	InitGame();


	while (!close_game) {
		if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
			close_game = true;
			continue;
		}
		UpdateDrawFrame();
	}
	UnloadGame();
	CloseWindow();
	return 0;
}