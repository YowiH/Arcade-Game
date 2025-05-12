#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "obstacle.h"

enum stage {
	DESERT,
	FOREST
};

class Map {
private:
	std::string M;
	char S;

	bool bush_frame;

	int powerUp_lifespan;
	bool obstacles_positioned;

	stage actual_stage;

	Texture2D bridge;

	// desert
	Texture2D dirt_grass;
	Texture2D dirt;
	Texture2D dirt_stones;
	Texture2D path;
	Texture2D path_stones;
	Texture2D river_desert1, river_desert2;
	Texture2D bush_spritesheet;
	Texture2D logs;

	// forest
	Texture2D grass;
	Texture2D flowers_grass;
	Texture2D path_grass;
	Texture2D path_grass2;
	Texture2D tall_grass;
	Texture2D green_bush_spritesheet;
	Texture2D stump;
	Texture2D river_forest1;
	Texture2D river_forest2;

	std::vector<Obstacle> obstacle_tracker{};
	std::vector<Obstacle> obstacle_pool{};
public:
	Map(const char* F, char C);

	std::string getStr();
	char getS();
	int get_powerUp_lifespan() const;
	stage get_actual_stage() const;
	std::vector<Obstacle> get_obstacle_tracker() const;
	std::vector<Obstacle> get_obstacle_pool() const;

	void change_bush_frame();
	void set_stage(stage s);
	void changeLevel(float frames_since_level_start, float level_length, int active_enemies, std::vector<Map>& map_list, int& level_index);
	void positionObstacle(float posX, float posY, const float tile_size);

	void load_assets();
	void unload_assets();
	void DrawMap(const float tile_size, const float left_margin);
};