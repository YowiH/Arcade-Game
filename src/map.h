#pragma once

#include "raylib.h"
#include "resource_dir.h"

#include <fstream>
#include <string>
#include <vector>

class Map {
private:

	int current_frame;
	float frame_time;
	float timer;

	Sound hurt;

	Texture2D dirt;
	Texture2D path;
	Texture2D dirt_stones;
	Texture2D dirt_grass;
	Texture2D bush_spritesheet;
	
	std::vector<Rectangle> bushes;
	int bush_frame;
	float bush_frame_time;
	float bush_frame_duration;
	
	std::vector<std::string> map_data;
public:
	Map();

	void load(float tile_size);
	void update(float delta_time);
	void draw(float tile_size, int tiles);

	bool check_collision(Rectangle player_rectangle);

	Sound get_hurt() const;

	~Map();
};