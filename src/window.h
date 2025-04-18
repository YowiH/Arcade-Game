#pragma once

#include "raylib.h"
#include "resource_dir.h"

#include <vector>

#include "player.h"
#include "bullet.h"
#include "enemy.h"

class Window {
private:
	int tiles;
	float tile_size;
	float width;
	float height;

	Player player;

	std::vector<Bullet> bullets;
	int fire_rate;
	int fire_cooldown;

	std::vector<Enemy> enemies;
	float enemy_spawn_timer;

public:
	Window(float ts);

	void load();
	void update();
	void draw();
	void unload();
};