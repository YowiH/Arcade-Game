#include "map.h"

void Audio::play_music() {
	PlayMusicStream(main_theme);
}

Map::Map(const char* F) {
	M = LoadFileText(F);
	obstacles_positioned = false;
	bush_frame = false;
	src = { 0 };
}

std::string Map::get_string() {
	return M;
}

Desert::Desert(const char* F) : Map(F) {

}

void Desert::Draw(const float tile_size, ) {
	int k = 0;
	if (!bush_frame) {
		src = { 0.0f, 0.0f, 16.0f, 16.0f }; //tile_size , tile_size
	}
	else {
		src = { 17.0f, 0.0f, 16.0f, 16.0f }; //tile_size , tile_size
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			switch (get_string()[k]) {
			case 'D':
				DrawTextureEx(dirt, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'M':
				DrawTextureEx(dirt_grass, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'S':
				DrawTextureEx(dirt_stones, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'P':
				DrawTextureEx(path, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				break;
			case 'B':
				DrawTexturePro(bush_spritesheet, src, { tile_size * j + (tile_size * 3), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
				break;
			case 'V':
				DrawTexturePro(bush_spritesheet, src, { tile_size * j + (tile_size * 3), tile_size * i + tile_size , tile_size, tile_size }, { 0,0 }, 0, WHITE);
				if (!obstacles_positioned) {
					obstacle_position(tile_size * j + (tile_size * 3), tile_size * i + tile_size);
				}
				break;
			case 'O':
				DrawTextureEx(logs, { tile_size * j + (tile_size * 3), tile_size * i + tile_size }, 0, tile_size / 16, WHITE);
				if (!obstacles_positioned) {
					obstacle_position(tile_size * j + (tile_size * 3), tile_size * i + tile_size);
				}
				break;
			default:
				break;
			}
			k++;
		}
	}
	obstacles_positioned = true;
}