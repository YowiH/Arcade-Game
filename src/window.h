#pragma once

#include "raylib.h"
#include "resource_dir.h"

#include <vector>

#include "player.h"
#include "bullet.h"

class Window {
private:
	int width;
	int height;
	int tile_size;

	int fire_rate;
	int fire_cooldown;

	Player player;
	std::vector<Bullet> bullets;

public:
	void load();
	void update();
	void draw();
	void unload();
};