#pragma once

#include "raylib.h"
#include "resource_dir.h"

#include "player.h"

class Window {
private:
	int tile_size;
	int width;
	int height;
	int fire_rate;
	int fire_rate_counter;

	bool environment_frame;

	Player player;

	// desert terrain
	Texture2D dirt_grass;
	Texture2D dirt;
	Texture2D dirt_stones;
	Texture2D path;
	Texture2D path_stones;
	Texture2D river_desert;
	Texture2D bush_spritesheet;
	Texture2D logs;

public:
	void create();
	void texturize();
	void update();
	void draw();
	void unload();
};