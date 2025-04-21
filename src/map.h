#pragma once

#include "raylib.h"
#include "resource_dir.h"

#include <fstream>
#include <string>
#include <vector>

class Map {
private:
	std::vector<std::string> map_data;

	int current_frame;
	float frame_time;
	float timer;

	Texture2D dirt;
	Texture2D path;
	Texture2D dirt_stones;
	Texture2D dirt_grass;
	Texture2D bush_spritesheet;
	
	int bush_frame;
	float bush_frame_time;
	float bush_frame_duration;
public:
	Map();

	Sound hurt;

	void load_map();
	void unload_map();

	void update(float delta_time);
	void draw(float tile_size, int tiles);

	void load_audio();
	void unload_audio();

	void load_textures();
	void unload_textures();

	~Map();
};