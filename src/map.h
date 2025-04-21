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

	Music music;
	Sound hurt;
	Sound shoot_1;
	Sound shoot_2;
	Sound shoot_3;
	Sound shoot_4;
	Sound shoot_5;
	Sound shoot_6;
	Sound shoot_7;
	Sound enemy_death_1;
	Sound enemy_death_2;
	Sound enemy_death_3;
	Sound enemy_death_4;

	Texture2D jopk_logo;
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

	Texture2D get_jopk_logo() const;
	Sound get_hurt() const;
	Sound get_shoot() const;
	Sound get_enemy_death() const;

	~Map();
};