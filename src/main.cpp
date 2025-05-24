#include <raylib.h>
#include <vector>
#include <resource_dir.h>
#include <iostream>
#include <string>

#include "map.h"
#include "obstacle.h"
#include "enemy.h"
#include "player.h"
#include "powerUp.h"

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

 static float zoom = 0.01f * (tile_size / 16);
 static bool zoom_completed = false;
 static float zoom_speed = 0.005f * (tile_size / 16);

//game over screen variables
//int selected_option = 0;
bool close_game;

//player varaibles
Player player;

//shooting
int fire_frame_counter = 0;
int fire_rate = 15;
Vector2 Shoot_dir;
int anim_dir; //variable to know what animation to play when shooting


//MAPS AND AREAS
int level_count = 0;
int powerUp_lifespan = 600;
bool obstacles_positioned = false;

//ENEMY SPAWNING VARIABLES
int active_enemies = 0;
int max_active_enemies = 5;
int enemy_creation_delay = 30;
int frames_since_enemy_spawn = 0;

struct trial_obs {
	bool alive = true;
	Vector2 position;
	Color color;
};

struct death_anim {
	Vector2 position;
	int frameCounter = 0;
};

//bullet vectors
std::vector<Bullet> bullet_tracker{};
std::vector<Bullet> bullet_pool{};

//enemy vectors
std::vector<Enemy> enemy_tracker{};
std::vector<Enemy> enemy_pool{};

//obstacle vector
std::vector<Obstacle> obstacle_tracker{};
std::vector<Obstacle> obstacle_pool{};

//power up vector

std::vector<PowerUp> powerUp_tracker{};
std::vector<PowerUp> powerUp_pool{};

//death animations
std::vector<death_anim> deathAnim_tracker{};
std::vector<death_anim> deathAnim_pool{};

std::vector<Map> map_list{};

//TEXTURES
//Create gloval varaibles for all textures so they can be used by the draw function and the load assets function

Texture2D title_screen;

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
Texture2D coffe;
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

void LoadAssets() {
	map_list = { Map("AREAS/area1_1.txt", 'D'), Map("AREAS/area1_2.txt", 'D'), Map("AREAS/area1_3.txt", 'D'), Map("AREAS/area1_4.txt", 'D'), Map("AREAS/area2_1.txt", 'F'), Map("AREAS/area2_2.txt" , 'F'), Map("AREAS/area2_3.txt" , 'F')};
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

	//CHARACTERS
	player_character_spritesheet = LoadTexture("player_character_spritesheet.png");
	player_character_death = LoadTexture("player_character_death_anim.png");
	bullet_player = LoadTexture("bullet.png");

	//ENEMIES
	orc_spritesheet = LoadTexture("orc_spritesheet.png");

	//DAMAGE & DEATH ANIMATIONS
	death_animation_enemy = LoadTexture("death_animation_enemy.png");

	//POWER UPS
	coin = LoadTexture("coin.png");
	extra_life = LoadTexture("extra_life.png");

	//UI
	timer = LoadTexture("clock.png");
	power_up_slot = LoadTexture("power_up_slot.png");
	title_screen = LoadTexture("title_screen.png");

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

	//characters
	UnloadTexture(player_character_spritesheet);

	//enemies
	UnloadTexture(orc_spritesheet);

	//damage and death animations
	UnloadTexture(death_animation_enemy);

	//power ups
	UnloadTexture(coin);
	UnloadTexture(extra_life);

	//sounds
	UnloadSound(shoot_fx);
	UnloadSound(enemy_death);
	UnloadSound(coin_sound);

	//music
	UnloadMusicStream(main_theme);

	CloseAudioDevice();
}

//INITAILIZE GAME

void InitGame() {
	SetTargetFPS(60);
	game_started = false;
	zoom_completed = false;
	player.set_hp(3);
	player.set_position({ left_margin + (area_size / 2), tile_size + (area_size * 3 / 4)});
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

void createEnemy() {
if (active_enemies < max_active_enemies && frames_since_enemy_spawn >= enemy_creation_delay) {
		randVal = GetRandomValue(0, 11);
		frames_since_enemy_spawn = 0;
		Vector2 pos;
		switch (randVal) {
			// up
		case 0:
			pos = { tile_size * 10, tile_size };
			break;
		case 1:
			pos = { tile_size * 11, tile_size };
			break;
		case 2:
			pos = { tile_size * 12, tile_size };
			break;

			// left
		case 3:
			pos = { left_margin, tile_size * 8 };
			break;
		case 4:
			pos = { left_margin, tile_size * 9 };
			break;
		case 5:
			pos = { left_margin, tile_size * 10 };
			break;

			// right
		case 6:
			pos = { tile_size * 19, tile_size * 8 };
			break;
		case 7:
			pos = { tile_size * 19, tile_size * 9 };
			break;
		case 8:
			pos = { tile_size * 19, tile_size * 10 };
			break;

			// down
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
			std::cout << "the random value has a wrong value" << std::endl;
			break;
		}
		if (enemy_pool.empty()) {
			Enemy enemy(pos);
			enemy_tracker.push_back(std::move(enemy));
		}
		else {
			enemy_tracker.push_back(std::move(enemy_pool.back()));
			enemy_tracker.back().set_hp(1);
			enemy_tracker.back().set_position({pos.x, pos.y});
			enemy_pool.pop_back();
		}
		active_enemies++;

	}
	else {
		frames_since_enemy_spawn++;
	}
}

void moveEnemies() {
	float magnitude;
	for (int i = 0; i < enemy_tracker.size(); i++) {
		magnitude = sqrt((player.get_rec().x - enemy_tracker[i].get_rec().x) * (player.get_rec().x - enemy_tracker[i].get_rec().x) + (player.get_rec().y - enemy_tracker[i].get_rec().y) * (player.get_rec().y - enemy_tracker[i].get_rec().y));
		Rectangle enemyRectangle = { enemy_tracker[i].get_rec().x + ((player.get_rec().x - enemy_tracker[i].get_rec().x) / magnitude) * enemy_tracker[i].get_speed(), enemy_tracker[i].get_rec().y + ((player.get_rec().y - enemy_tracker[i].get_rec().y) / magnitude) * enemy_tracker[i].get_speed() };
		enemy_tracker[i].set_rec(enemyRectangle);
	}
}

void spawnPowerUp(float x, float y) {
	randVal = GetRandomValue(1, 5);
	if (randVal == 1) {
		char C;
		randVal = GetRandomValue(0, 1);
		switch (randVal) {
		case 0: //extra life
			C = 'U';
			break;
		case 1: //coin
			C = 'O';
			break;
		default:
			std::cout << "type of power up not recognised" << std::endl;
			C = '.';
			break;
		}

		if (powerUp_pool.empty()) {
			//en crea un
			PowerUp powerUp({ x, y });
			powerUp.set_type(C);
			powerUp_tracker.push_back(std::move(powerUp));
		}
		else {
			// agafa un del pool
			powerUp_tracker.push_back(std::move(powerUp_pool.back()));
			powerUp_tracker.back().set_position({ x , y });
			powerUp_tracker.back().set_type(C);
			powerUp_tracker.back().set_despawn_timer(0);
			powerUp_pool.pop_back();
		}

	}
}

void createDeathAnimation(Vector2 pos) {
	PlaySound(enemy_death);
	if (deathAnim_pool.empty()) {
		death_anim d;
		d.position = pos;
		deathAnim_tracker.push_back(std::move(d));
	}
	else {
		deathAnim_tracker.push_back(std::move(deathAnim_pool.back()));
		deathAnim_tracker.back().position = pos;
		deathAnim_tracker.back().frameCounter = 0;
		deathAnim_pool.pop_back();
	}
}

void updateDeathAnimations() {
	//eliminar els objectes que porten 10 segons en joc
	for (int i = deathAnim_tracker.size() - 1; i >= 0; i--) {
		//eliminar els objectes que porten 10 segons en joc
		deathAnim_tracker[i].frameCounter++;

		if (deathAnim_tracker[i].frameCounter >= 60 * 10) {
			//save the death animation in the pool
			deathAnim_pool.push_back(std::move(deathAnim_tracker[i]));
			//borrar death animation
			auto& j = deathAnim_tracker.begin() + i;
			deathAnim_tracker.erase(j);
		}
	}

}

//BULLETS MANAGER
void bullet_update() {
	//get the lenght of the bullet vector
	//update bullet's position
	for (int i = 0; i < bullet_tracker.size(); i++) {
		Bullet& b = bullet_tracker[i];
		if (b.get_velocity().x != 0 && b.get_velocity().y != 0) {
			b.set_position({ b.get_rec().x + b.get_velocity().x * b.get_speed() * 0.707f, b.get_rec().y + b.get_velocity().y * b.get_speed() * 0.707f });
		}
		else {
			b.set_position({ b.get_rec().x + b.get_velocity().x * b.get_speed(), b.get_rec().y + b.get_velocity().y * b.get_speed() });
		}
	}

	//destroy bullets
	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
		//iterate and check all bullets if they are ouside of the map (should I check if they colisioned?, might only have to check the first shot if we don't check colisions)
		if ((bullet_tracker[i].get_rec().x <= left_margin || bullet_tracker[i].get_rec().x >= area_size + (left_margin) || bullet_tracker[i].get_rec().y <= tile_size || bullet_tracker[i].get_rec().y >= area_size + tile_size)) { //&& bullet_tracker[i] != NULL
			//save the bullet in the pool
			bullet_pool.push_back(std::move(bullet_tracker[i]));
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}

	}
}

void bullet_draw(Texture2D bullet_player) {
	int bullet_amount = bullet_tracker.size();
	for (int i = 0; i < bullet_amount; i++) {
		DrawTextureEx(bullet_player, bullet_tracker[i].get_position(), 0, tile_size / 16, WHITE);
	}
}

void bullet_attack(std::vector<Enemy>& enemy_tracker, std::vector<Enemy>& enemy_pool, std::vector<Bullet>& bullet_tracker, int& active_enemies) {
	//comprovar totes les bales per tots els enemics
	for (int i = enemy_tracker.size() - 1; i >= 0; i--) {
		for (int j = bullet_tracker.size() - 1; j >= 0; j--) {
			if (enemy_tracker.size() - 1 >= i && bullet_tracker.size() - 1 >= j && !enemy_tracker.empty() && !bullet_tracker.empty()) {
				if (CheckCollisionCircles({ bullet_tracker[j].get_rec().x + tile_size / 2, bullet_tracker[j].get_rec().y + tile_size / 2 }, tile_size / 8, { enemy_tracker[i].get_rec().x + tile_size / 2, enemy_tracker[i].get_rec().y + tile_size / 2 }, tile_size / 2)) { //MASSIVE ERROR
					//save the bullet in the pool
					bullet_pool.push_back(std::move(bullet_tracker[j]));
					//borrar bullet
					auto& k = bullet_tracker.begin() + j;
					bullet_tracker.erase(k);

					//reduce hit enemy hitpoints
					enemy_tracker[i].set_hp(enemy_tracker[i].get_hp() - player.get_damage());
					//kill enemy if hitpoints are 0 or lower
					if (enemy_tracker[i].get_hp() <= 0) {

						//save the enemy in the pool
						enemy_pool.push_back(std::move(enemy_tracker[i]));

						//mirar si es crea un power up
						spawnPowerUp(enemy_tracker[i].get_rec().x, enemy_tracker[i].get_rec().y);

						//crea un death animations object
						createDeathAnimation(enemy_tracker[i].get_position());

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

void bullet_collision(std::vector<Obstacle>& obstacle_tracker) {
	for (int j = 0; j < obstacle_tracker.size(); j++) {
		for (int i = bullet_tracker.size() - 1; i >= 0; i--) {

			if (CheckCollisionCircles({ bullet_tracker[i].get_rec().x + tile_size / 2, bullet_tracker[i].get_rec().y + tile_size / 2 }, tile_size / 8, { obstacle_tracker[j].get_rec().x + tile_size / 2, obstacle_tracker[j].get_rec().y + tile_size / 2 }, tile_size / 2)) {
				//save the bullet in the pool
				bullet_pool.push_back(std::move(bullet_tracker[i]));
				//borrar bullet
				auto& k = bullet_tracker.begin() + i;
				bullet_tracker.erase(k);
			}
		}
	}
}

//POWER UP
void powerUpUpdate() {
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
		powerUp_tracker[i].add_despawn_timer(1);
		if (powerUp_tracker[i].get_despawn_timer() > powerUp_lifespan) {
			//guardar power up al pool
			powerUp_pool.push_back(std::move(powerUp_tracker[i]));
			//borrar powerUp
			auto& k = powerUp_tracker.begin() + i;
			powerUp_tracker.erase(k);
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
	if (player.get_mov_dir() != 0){ //si el player s'està movent
		player.add_walk_anim_counter(1);
		if (player.get_walk_anim_counter() % 6 == 0) {
			player.set_right_foot(!player.get_right_foot());
			//play walking sound

		}
	}
	else {
		player.set_walk_anim_counter(0);
	}

	if (player.get_is_dying()) {
		player.add_death_anim(1);
	}

	//ENEMIES
	for (int i = 0; i < enemy_tracker.size(); i++) {
		enemy_tracker[i].add_animation_counter(1);
		if (enemy_tracker[i].get_animation_counter() % 12 == 0) {
			enemy_tracker[i].set_right_foot(!enemy_tracker[i].get_right_foot());
			enemy_tracker[i].set_animation_counter(0);
		}
	}
}

void changeLevel() {
	if (IsKeyDown('N') && frames_since_level_start >= level_length && active_enemies == 0) {//checks if the timer is over and if there aren't any enemies in order to advance
		frames_since_level_start = 0;
		//delete all obstacles
		obstacle_tracker.clear();
		obstacles_positioned = false;
		if (level_count + 1 < map_list.size()) {//adds one to the index of the map vector
			level_count++;
		}
		else {//repeats to the first level if player is on the last
			level_count = 0;
		}
	}
}
//UPDATE GAME
void UpdateGame() {//update variables and positions

	if (game_started && player.get_hp() >= 0) {
		//MUSIC
		UpdateMusicStream(main_theme);
		if (!player.get_is_dying()) {

			//CREATE ENEMIES
			if (frames_since_level_start < level_length) {
				createEnemy();
				//incrementar variable temps
				frames_since_level_start++;
			}

			//MOVEMENT
			player.move();

			moveEnemies();

			//SHOOTING BULLETS
			player.shoot(shoot_fx, bullet_tracker, bullet_pool);

			//update bullets
			bullet_update();

			//update power ups
			powerUpUpdate();

			//COLLISIONS
			//player-enemies
			player.be_attacked(player_death, enemy_tracker, enemy_pool, active_enemies, powerUp_tracker, powerUp_pool, bullet_tracker, bullet_pool);

			//player-obstacles
			player.collide(obstacle_tracker);

			//bullet-enemies
			bullet_attack(enemy_tracker, enemy_pool, bullet_tracker, active_enemies);

			//bullets-obstacles
			bullet_collision(obstacle_tracker);

			//player power-up colisions
			player.collect(powerUp_tracker, powerUp_pool, power_up_pick_up, coin_sound);
		}

		//update death animations
		updateDeathAnimations();

		//ANIMATIONS
		animationManager();

		changeLevel();
	}
	else if (!game_started) {//start of the game
		UpdateStartScreen();
	}
	else {//death of the player
		UpdateDeathScreen();
	}

	
}

//DRAW
void DrawUI() {
	//draw clock
	DrawTextureEx(timer, { left_margin, 0 }, 0, tile_size / 16, WHITE);

	//draw life 
	DrawTextureEx(extra_life, { 0, left_margin }, 0, (tile_size / 16) * 1.25, WHITE);
	//write actual lives
	DrawText(TextFormat("X%i", player.get_hp()), tile_size + tile_size / 2, left_margin + tile_size / 4, 10 * (tile_size / 16), WHITE);

	//draw coins
	DrawTextureEx(coin, { 0, tile_size * 4 }, 0, (tile_size / 16) * 1.25, WHITE);
	//write actual coins
	DrawText(TextFormat("X%i", player.get_coins()), tile_size + tile_size/2, tile_size * 4 + tile_size / 4, 10 * (tile_size / 16), WHITE);
	
	//draw power up slot
	DrawTextureEx(power_up_slot, { tile_size, tile_size}, 0, (tile_size / 16) * 1.25, WHITE);

	//draw level bar
	int length = area_size - (area_size * (frames_since_level_start / level_length));
	DrawRectangle(tile_size * 4, static_cast<int>(tile_size * 0.45), length, static_cast<int>(tile_size * 0.4), GREEN);
}
void DrawEnemies() {
	for (int i = 0; i < enemy_tracker.size(); i++) {
		if (enemy_tracker[i].get_right_foot()) {
			src = { 0, 0, 16, 16 };
		}
		else {
			src = { 16, 0, 16, 16 };
		}
		DrawTexturePro(orc_spritesheet, src, { enemy_tracker[i].get_rec().x, enemy_tracker[i].get_rec().y , tile_size, tile_size}, {0,0}, 0, WHITE);
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
		switch (powerUp_tracker[i].get_type()) {
		case 'U':
			DrawTextureEx(extra_life, { powerUp_tracker[i].get_position()}, 0, tile_size / 16, WHITE);
			break;
		case 'O':
			DrawTextureEx(coin, { powerUp_tracker[i].get_position()}, 0, tile_size / 16, WHITE);
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

	DrawTexturePro(title_screen,
		{ 0,0,(16 * 6), (16 * 5) },
		{ area_size / 2 - ((tile_size) * 6 * zoom) / 2, area_size / 2 - ((tile_size) * 5 * zoom) / 2, (tile_size * 6 * zoom), (tile_size * 5 * zoom) },
		{ 0, 0},
		0, WHITE);
}
void DrawGameOverScreen() {
	ClearBackground(BLACK);
	if (!close_game) {
		DrawText("Game Over", static_cast<int>(area_size / 2 - 80), static_cast<int>(area_size / 2 - 40), 40, WHITE);

		DrawText("Quit", static_cast<int>(area_size / 2 - 80), static_cast<int>(area_size / 2 + 80), 20, RED);

		//DrawText("Restart", area_size / 2 - 80, area_size / 2 + 40, 20, (selected_option == 0) ? RED : WHITE);
		//DrawText("Quit", area_size / 2 - 80, area_size / 2 + 80, 20, (selected_option == 1) ? RED : WHITE);
		//Vector2 arrowPos = { area_size / 2 - 70, area_size / 2 + 40 + (selected_option * 40) };
		//DrawTriangleLines(arrowPos, { arrowPos.x + 10, arrowPos.y - 5 }, { arrowPos.x + 10, arrowPos.y + 5 }, WHITE);
	}
}
void DrawGame() {//draws the game every frame
	BeginDrawing();
	if (game_started && player.get_hp() >= 0) {
		//draw background
		ClearBackground(BLACK);

		//draw map
		map_list[level_count].draw(bush_frame, obstacles_positioned, obstacle_pool, obstacle_tracker, desert_textures, forest_textures);

		//draw enmie death animation
		DrawDeathAnimations();

		//draw UI
		DrawUI();

		//draw player
		if (!player.get_is_dying()) {
			player.draw(player_character_spritesheet);
		}
		else {
			player.draw_death(player_character_death);
		}

		//draw enemies
		DrawEnemies();

		//draw power ups
		drawPowerUps();

		//draw bullets
		bullet_draw(bullet_player);

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
	InitWindow(static_cast<int>(area_size + (left_margin)), static_cast<int>(area_size + tile_size), "Journey of the prairie king");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");


	//start some values
	InitGame();


	while (!close_game) {
		UpdateDrawFrame();
	}
	UnloadGame();
	CloseWindow();
	return 0;
}