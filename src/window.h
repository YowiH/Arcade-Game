#pragma once

#include "raylib.h"
#include "resource_dir.h"

#include <fstream>
#include <vector>
#include <string>

#include "player.h"
#include "bullet.h"
#include "enemy.h"

class Window {
private:
	int tiles;
	float tile_size;
	float width;
	float height;

	bool is_game_over;

	Player player;

	std::vector<Bullet> bullets;
	int fire_rate;
	int fire_cooldown;

	std::vector<Enemy> enemies;
	float enemy_spawn_timer;
	float enemy_spawn_rate;

	Sound hurt;

	std::vector<std::string> map_data;

	Texture2D dirt;
	Texture2D path;

public:
	Window(float ts);

	void load();
	void update();
	void draw();
	void unload();
};