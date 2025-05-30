#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <iostream>

#include "obstacle.h"

class Map {
private:
	std::string M;
	char S;
public:
	Map() = default;
	Map(const char* F, char C);
	std::string getStr();
	char getS();
	void draw(const float tile_size, const float left_margin, bool bush_frame, bool& obstacles_positioned, std::vector<Obstacle>& obstacle_pool, std::vector<Obstacle>& obstacle_tracker, const std::vector<Texture2D>& desert_textures, const std::vector<Texture2D>& forest_textures, const std::vector<Texture2D>& cementery_textures);
};
