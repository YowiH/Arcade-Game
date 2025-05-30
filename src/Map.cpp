#include "map.h"
#include "obstacle.h"
Map::Map(const char* F, char C) {
    char* rawText = LoadFileText(F);
    if (!rawText) {
        TraceLog(LOG_ERROR, "Failed to load map file: %s", F);
        throw std::runtime_error("Could not read map file.");
    }
    M = std::string(rawText);
    UnloadFileText(rawText);

    // Remove whitespace and non-tile characters (like \n, \r)
    M.erase(std::remove_if(M.begin(), M.end(), [](unsigned char c) {
        return std::isspace(c);
        }), M.end());

	S = C;
}
std::string Map::getStr() {
	return M;
}
char Map::getS() {
	return S;
}
void Map::draw(const float tile_size, const float left_margin, bool bush_frame, bool& obstacles_positioned, std::vector<Obstacle>& obstacle_pool, std::vector<Obstacle>& obstacle_tracker, const std::vector<Texture2D>& desert_textures, const std::vector<Texture2D>& forest_textures, const std::vector<Texture2D>& cementery_textures) {
    Rectangle src;
    if (bush_frame == 0) {
        src = { 0.0f, 0.0f, 16.0f, 16.0f };
    }
    else {
        src = { 17.0f, 0.0f, 16.0f, 16.0f };
    }

    std::vector<Texture2D> textures;
    switch (S) {
        case 'D': textures = desert_textures; break;
        case 'F': textures = forest_textures; break;
		case 'C': textures = cementery_textures; break;
        default: break;
    }


	if (S == 'C') {
        int index = 0;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                if (index >= M.size()) break;

                char tile = M[index++];
                Vector2 pos = { tile_size * j + left_margin, tile_size * i + tile_size };

                switch (tile) {
                case 'D':
                    DrawTextureEx(textures[0], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'M':
                    DrawTextureEx(textures[1], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'S':
                    DrawTextureEx(textures[2], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'P':
                    DrawTextureEx(textures[3], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'B':
                    DrawTexturePro(textures[4], src, { pos.x, pos.y, tile_size, tile_size }, { 0, 0 }, 0, WHITE); break;
                case 'V':
                    DrawTexturePro(textures[4], src, { pos.x, pos.y, tile_size, tile_size }, { 0, 0 }, 0, WHITE);
                    if (!obstacles_positioned && obstacle_pool.empty()) {
                        Obstacle obs(pos.x, pos.y, tile_size, 'V');
                        obstacle_tracker.push_back(obs);
                    }
                    break;
                default: break;
                }
            }
        }
        obstacles_positioned = true;
	}

    else {
        int index = 0;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                if (index >= M.size()) break;

                char tile = M[index++];
                Vector2 pos = { tile_size * j + left_margin, tile_size * i + tile_size };

                switch (tile) {
                case 'D':
                    DrawTextureEx(textures[0], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'M':
                    DrawTextureEx(textures[1], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'S':
                    DrawTextureEx(textures[2], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'N':
                    DrawTextureEx(textures[3], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'P':
                    DrawTextureEx(textures[4], pos, 0, tile_size / 16, WHITE);
                    break;
                case 'B':
                    DrawTexturePro(textures[5], src, { pos.x, pos.y, tile_size, tile_size }, { 0, 0 }, 0, WHITE); break;
                case 'V':
                    DrawTexturePro(textures[5], src, { pos.x, pos.y, tile_size, tile_size }, { 0, 0 }, 0, WHITE);
                    if (!obstacles_positioned && obstacle_pool.empty()) {
                        Obstacle obs(pos.x, pos.y, tile_size, 'V');
                        obstacle_tracker.push_back(obs);
                    }
                    break;
                case 'O':
                    DrawTextureEx(textures[6], pos, 0, tile_size / 16, WHITE);
                    if (!obstacles_positioned && obstacle_pool.empty()) {
                        Obstacle obs(pos.x, pos.y, tile_size, 'O');
                        obstacle_tracker.push_back(obs);
                    }
                    break;
                case 'R':
                    DrawTextureEx(textures[7], pos, 0, tile_size / 16, WHITE);
                    if (!obstacles_positioned && obstacle_pool.empty()) {
                        Obstacle obs(pos.x, pos.y, tile_size, 'R');
                        obstacle_tracker.push_back(obs);
                    }
                    break;
                case 'T':
                    DrawTextureEx(textures[8], pos, 0, tile_size / 16, WHITE);
                    if (!obstacles_positioned && obstacle_pool.empty()) {
                        Obstacle obs(pos.x, pos.y, tile_size, 'T');
                        obstacle_tracker.push_back(obs);
                    }
                    break;
                case 'I':
                    DrawTextureEx(textures[9], pos, 0, tile_size / 16, WHITE);
                    break;
                default: break;
                }
            }
        }

        obstacles_positioned = true;
    }
}