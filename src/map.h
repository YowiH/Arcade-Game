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

public:
	Map();

	Sound hurt;

	void load_map();
	void unload_map();

	void draw(float tile_size, int tiles);

	void load_audio();
	void unload_audio();

	void load_textures();
	void unload_textures();

	~Map();
};