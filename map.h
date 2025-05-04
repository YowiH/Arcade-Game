#pragma once
#include <raylib.h>
#include <string>
#include <vector>

class Audio {
private:
	Music main_theme;

	Sound shoot_fx;
	Sound enemy_death;
	Sound power_up_pick_up;
	Sound coin_sound;
	Sound player_death;
public:
	void play_music();
};

class Map {
protected:
	Rectangle src;
	std::string M;

	bool obstacles_positioned;
	bool bush_frame;
public:
	Map() = default;
	Map(const char* F);
	std::string get_string();
	virtual void Draw() {};
};

class Desert : public Map {
private:
	Texture2D dirt_grass;
	Texture2D dirt;
	Texture2D dirt_stones;
	Texture2D path;
	Texture2D path_stones;
	Texture2D river_desert;
	Texture2D bush_spritesheet;
	Texture2D logs;
public:
	Desert(const char* F);
	void Draw();
};

class Forest : public Map {
	Texture2D grass;
	Texture2D flowers_grass;
	Texture2D path_grass;
	Texture2D path_grass2;
	Texture2D tall_grass;
	Texture2D river_forest;
	Texture2D green_bush_spritesheet;
	Texture2D stump;
};

class Cementery : public Map {
	Texture2D cementery_floor;
	Texture2D cementery_gravel;
	Texture2D cementery_path;
	Texture2D stone_floor;
	Texture2D tombstone;
};

std::vector<Map> maps;