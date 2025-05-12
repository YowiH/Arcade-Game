#include "map.h"

Map::Map(const char* F, char C) {
	M = LoadFileText(F);
	S = C;

	powerUp_lifespan = 600;
	obstacles_positioned = false;
	bush_frame = false;
}
std::string Map::getStr() {
	return M;
}
char Map::getS() {
	return S;
}

int Map::get_powerUp_lifespan() const {
	return powerUp_lifespan;
}

std::vector<Obstacle> Map::get_obstacle_tracker() const {
	return obstacle_tracker;
}
std::vector<Obstacle> Map::get_obstacle_pool() const {
	return obstacle_pool;
}


stage Map::get_actual_stage() const {
	return actual_stage;
}

void Map::set_stage(stage s) {
	actual_stage = s;
}

void Map::changeLevel(float frames_since_level_start, float level_length, int active_enemies, std::vector<Map>& map_list, int& level_index) {

	switch (getS()) {
	case 'D':
		set_stage(DESERT);
		break;
	case 'F':
		set_stage(FOREST);
		break;
	}

	if (IsKeyDown('N') && frames_since_level_start >= level_length && active_enemies == 0) {//checks if the timer is over and if there aren't any enemies in order to advance
		frames_since_level_start = 0;
		//delete all obstacles
		obstacle_tracker.clear();
		obstacles_positioned = false;

		if (level_index >= map_list.size()) {
			level_index = 0;
		}

		level_index++;
	}
}


void Map::positionObstacle(float posX, float posY, const float tile_size) {
	if (obstacle_pool.empty()) {
		Obstacle obs;
		obs.rec = { posX, posY, tile_size, tile_size };
		obstacle_tracker.push_back(obs);
	}
	//afegir codi de bullet pool
}

void Map::load_assets() {
	dirt_grass = LoadTexture("dirt_grass.png");
	dirt = LoadTexture("dirt.png");
	dirt_stones = LoadTexture("dirt_stones.png");
	bush_spritesheet = LoadTexture("bush_spritesheet.png");
	path = LoadTexture("path.png");
	path_stones = LoadTexture("path_stones.png");
	bridge = LoadTexture("bridge.png");
	logs = LoadTexture("logs.png");
	river_desert1 = LoadTexture("river_desert1.png");
	river_desert2 = LoadTexture("river_desert2.png");

	//forest
	grass = LoadTexture("grass.png");
	flowers_grass = LoadTexture("flowers_grass.png");
	green_bush_spritesheet = LoadTexture("green_bush_spritesheet.png");
	path_grass = LoadTexture("path_grass.png");
	path_grass2 = LoadTexture("path_grass2.png");
	tall_grass = LoadTexture("tall_grass.png");
	stump = LoadTexture("stump.png");
	river_forest1 = LoadTexture("river_forest1.png");
	river_forest2 = LoadTexture("river_forest2.png");
}

void Map::DrawMap(const float tile_size, const float left_margin) {
	int k = 0;
	Rectangle src;
	if (bush_frame == false) {
		src = { 0.0f, 0.0f, 16.0f, 16.0f };
	}
	else {
		src = { 17.0f, 0.0f, 16.0f, 16.0f };
	}
	switch (actual_stage) {
	case DESERT:
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				switch (getStr()[k]) {
				case 'D':
					DrawTextureEx(dirt, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'M':
					DrawTextureEx(dirt_grass, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'S':
					DrawTextureEx(dirt_stones, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'N':
					DrawTextureEx(path_stones, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'P':
					DrawTextureEx(path, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'B':
					DrawTexturePro(bush_spritesheet, src, { tile_size * j + (left_margin), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
					break;
				case 'V':
					DrawTexturePro(bush_spritesheet, src, { tile_size * j + (left_margin), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'O':
					DrawTextureEx(logs, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'R':
					DrawTextureEx(river_desert1, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'T':
					DrawTextureEx(river_desert2, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'I':
					DrawTextureEx(bridge, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				default:
					break;
				}
				k++;
			}
		}
		break;
	case FOREST:
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				switch (getStr()[k]) {
				case 'D':
					DrawTextureEx(grass, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'M':
					DrawTextureEx(flowers_grass, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'S':
					DrawTextureEx(tall_grass, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'P':
					DrawTextureEx(path_grass, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'N':
					DrawTextureEx(path_grass2, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				case 'B':
					DrawTexturePro(green_bush_spritesheet, src, { tile_size * j + (left_margin), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
					break;
				case 'V':
					DrawTexturePro(green_bush_spritesheet, src, { tile_size * j + (left_margin), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'O':
					DrawTextureEx(stump, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'R':
					DrawTextureEx(river_forest1, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'T':
					DrawTextureEx(river_forest2, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					if (!obstacles_positioned) {
						positionObstacle(tile_size * j + (left_margin), tile_size * i + tile_size, tile_size);
					}
					break;
				case 'I':
					DrawTextureEx(bridge, { tile_size * j + (left_margin), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
					break;
				default:
					break;
				}
				k++;
			}
		}
		break;
	}

	obstacles_positioned = true;
}

void Map::change_bush_frame() {
	bush_frame = !bush_frame;
}



void Map::unload_assets() {

	//dessert
	UnloadTexture(dirt_grass);
	UnloadTexture(dirt);
	UnloadTexture(bush_spritesheet);
	UnloadTexture(path);
	UnloadTexture(path_stones);
	UnloadTexture(logs);
	UnloadTexture(river_desert1);
	UnloadTexture(river_desert2);

	//forest
	UnloadTexture(grass);
	UnloadTexture(flowers_grass);
	UnloadTexture(path_grass);
	UnloadTexture(path_grass2);
	UnloadTexture(green_bush_spritesheet);
	UnloadTexture(tall_grass);
	UnloadTexture(stump);
	UnloadTexture(river_forest1);
	UnloadTexture(river_forest2);
}