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

//CENTRAL VARAIBLES
bool game_started = false;
const float tile_size = 32;
const float area_size = tile_size * 16;
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
Vector2 player_size{ tile_size, tile_size};
Vector2 player_pos{ tile_size * 3 + (area_size / 2), tile_size + (area_size * 3 / 4)};
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


//MAPS AND AREAS
Map active_map;
int level_count = 0;
int powerUp_lifespan = 600;
bool obstacles_positioned = false;

//ENEMY SPAWNING VARIABLES
int active_enemies, max_active_enemies = 5;
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
	float speed = 1.25;
	Vector2 velocity;
	Vector2 position;
	int hp;
	int anim_counter = 0;
	bool right_foot;
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

struct Obsticle {
	Rectangle rec;
};
struct trial_obs tri;

struct death_anim {
	Vector2 position;
	int frameCounter = 0;
};

//bullet vectors
std::vector<bullet> bullet_tracker{};
std::vector<bullet> bullet_pool{};

//enemy vectors
std::vector<enemy> enemy_tracker{};
std::vector<enemy> enemy_pool{};

//obstacle vector
std::vector<Obsticle> obsticle_tracker{};
std::vector<Obsticle> obsticle_pool{};

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
	player_character_death = LoadTexture("player_character_death_anim.png");

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

	CloseAudioDevice;
}

//INITAILIZE GAME

void InitGame() {
	SetTargetFPS(60);
	game_started = false;
	zoom_completed = false;
	map_list = { Map("AREAS/area1_1.txt"), Map("AREAS/area1_2.txt"), Map("AREAS/area1_3.txt") };
	active_map = map_list[level_count];
	level_count++;
	lives = 3;
	player_pos = {tile_size + 3 + (area_size / 2), tile_size + (area_size * 3 / 4)};
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
			b.position = { player_pos.x + tile_size / 4, player_pos.y + tile_size / 4 };
			b.velocity = Shoot_dir;
			bullet_tracker.push_back(b);
		}
		else {
			bullet_tracker.push_back(bullet_pool.back());
			bullet_tracker.back().damage = damage;
			bullet_tracker.back().position = { player_pos.x + tile_size / 4 , player_pos.y + tile_size / 4 };
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

void bulletUpdate() {
	//get the lenght of the bullet vector
	//update bullet's position
	for (int i = 0; i < bullet_tracker.size(); i++) {
		bullet& b = bullet_tracker[i];
		b.position = Vector2{ b.position.x + b.velocity.x * b.speed, b.position.y + b.velocity.y * b.speed };
	}

	//destroy bullets
	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
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


//COLLISIONS
void player_enemyColl() {
	bool already_damaged = false;
	int i = 0;
	while (!already_damaged && i < enemy_tracker.size()) {
		if (CheckCollisionCircles(player_pos, tile_size / 2, enemy_tracker[i].position, tile_size / 2)) {
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


void restrainPlayerMovement(float x, float y) {
	//define two vectors and use them to conclude the direction of the colision
	Vector2 v = { 0, 0 };
	Vector2 u = { 0, 0 };

	//inici vector
	float x0 = player_pos.x;
	float y0 = player_pos.y;

	//v va des del centre del obstacle (cantonada esquerra de dalt) al centre del jugador
	v = { x0 - x, y0 - y };
	//u va des del centre del jugador al centre de l'obstacle
	u = { tile_size / 2, 0 };
	float theta = GetAngle(u, v);

	if (theta >= PI / 4 && theta < 3 * PI / 4) {
		//colisio per dalt
		yNegBlock = true;
	}
	else if (theta >= 3 * PI / 4 && theta < 5 * PI / 4) {
		//colisió per l'esquerra
		xNegBlock = true;
	}
	else if (theta >= 5 * PI / 4 && theta < 7 * PI / 4) {
		//colisio per baix
		yPosBlock = true;
	}
	else if (theta >= 7 * PI / 4 || theta < PI / 4) {
		//colisio per la dreta
		xPosBlock = true;
	}
	else {
		cout << "error when calculating the angle" << endl;
	}

	return;

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
				if (CheckCollisionCircles(bullet_tracker[j].position, tile_size / 4, enemy_tracker[i].position, tile_size / 2)) { //MASSIVE ERROR
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

						//mirar si es crea un power up
						spawnPowerUp(enemy_tracker[i].position.x, enemy_tracker[i].position.y);

						//crea un death animations object
						createDeathAnimation(enemy_tracker[i].position);

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

void player_powerUpColl() {
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--){
		if (CheckCollisionCircles(player_pos, tile_size / 2, { powerUp_tracker[i].position.x + tile_size / 3, powerUp_tracker[i].position.y + tile_size / 3 }, tile_size / 3)) {
			switch (powerUp_tracker[i].type) {
			case 'U': //vida extra
				lives++;
				PlaySound(power_up_pick_up);
				break;
			case 'O'://monedes
				coins++;
				PlaySound(coin_sound);
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
	if (IsKeyDown('N') && frames_since_level_start >= level_length && active_enemies == 0) {//checks if the timer is over and if there aren't any enemies in order to advance
		frames_since_level_start = 0;
		//delete all obstacles
		obsticle_tracker.clear();
		obstacles_positioned = false;
		if (level_count < map_list.size()) {//adds one to the index of the map vector
			active_map = map_list[level_count];
			level_count++;
		}
		else {//repeats to the first level if player is on the last
			level_count = 0;
			active_map = map_list[level_count];
			level_count++;
		}
	}
}

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

			//update power ups
			powerUpUpdate();

			//COLLISIONS
			//player-enemies
			player_enemyColl();

			//player-obstacles
			player_obsticleColl();

			//bullet-enemies
			bullet_enemyColl();

			//bullets-obstacles
			bullet_obsticleColl();

			//player power-up colisions
			player_powerUpColl();
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


void positionObsticle(float posX, float posY) {
	if (obsticle_pool.empty()) {
		Obsticle obs;
		obs.rec = {posX, posY, tile_size, tile_size};
		obsticle_tracker.push_back(obs);
	}
	//afegir codi de bullet pool
}

//DRAW
void DrawPlayer() {
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
		player_pos = { tile_size + 3 + (area_size / 2), tile_size + (area_size * 3 / 4) };
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
			case 'V':
				DrawTexturePro(bush_spritesheet, src, { tile_size * j + (tile_size * 3), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
				if (!obstacles_positioned) {
					positionObsticle(tile_size * j + (tile_size * 3), tile_size * i + tile_size);
				}
				break;
			case 'O':
				DrawTextureEx(logs, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				if (!obstacles_positioned) {
					positionObsticle(tile_size * j + (tile_size * 3), tile_size * i + tile_size);
				}
				break;
			default:
				break;
			}
			k++;
		}
	}
	obstacles_positioned = true;
}
void DrawUI() {
	//draw clock
	DrawTextureEx(timer, { tile_size * 3, 0 }, 0, tile_size / 16, WHITE);

	//draw life 
	DrawTextureEx(extra_life, { 0, tile_size * 3 }, 0, (tile_size / 16) * 1.25, WHITE);
	//write actual lives
	DrawText(TextFormat("X%i", lives), tile_size + tile_size / 2, tile_size * 3 + tile_size / 4, 10 * (tile_size / 16), WHITE);

	//draw coins
	DrawTextureEx(coin, { 0, tile_size * 4 }, 0, (tile_size / 16) * 1.25, WHITE);
	//write actual coins
	DrawText(TextFormat("X%i", coins), tile_size + tile_size/2, tile_size * 4 + tile_size / 4, 10 * (tile_size / 16), WHITE);
	
	//draw power up slot
	DrawTextureEx(power_up_slot, { tile_size, tile_size}, 0, (tile_size / 16) * 1.25, WHITE);

	//draw level bar
	int leangth = area_size - (area_size * (frames_since_level_start / level_length));
	DrawRectangle(tile_size * 4, tile_size * 0.45, leangth, tile_size * 0.4, GREEN);
}
void DrawEnemies() {
	int enemy_amount = enemy_tracker.size();
	for (int i = 0; i < enemy_amount; i++) {
		if (enemy_tracker[i].right_foot) {
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
	if (game_started && lives >= 0) {
		//draw background
		ClearBackground(BLACK);

		//draw map
		DrawMap();

		//draw enmie death animation
		DrawDeathAnimations();

		//draw UI
		DrawUI();

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
	InitWindow(area_size + (tile_size * 3), area_size + tile_size, "Journey of the prairie king");

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
	/*
	// cleanup
	// unload our texture so it can be cleaned up

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
	*/
}
