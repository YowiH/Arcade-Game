#include "map.h"

Map::Map() {
    current_frame = 0;
    frame_time = 0.0f;
    timer = 0.2f;

    hurt = Sound{ 0 };

    dirt = Texture2D{ 0 };
    path = Texture2D{ 0 };
    dirt_stones = Texture2D{ 0 };
    dirt_grass = Texture2D{ 0 };
    bush_spritesheet = Texture2D{ 0 };

    bush_frame = 0;
    bush_frame_time = 0.0f;
    bush_frame_duration = 0.6f;
}

void Map::load_map() {
    std::ifstream map_file("areas/1-1.txt");
    std::string line;
    while (std::getline(map_file, line)) {
        std::string cleaned;

        for (char c : line) {
            if (c != ' ') {
                cleaned += c;
            }
        }
        if (!cleaned.empty()) {
            map_data.push_back(cleaned);
        }
    }
}

void Map::unload_map() {
	map_data.clear();
}

void Map::update(float delta_time) {
    bush_frame_time += delta_time;
    if (bush_frame_time >= bush_frame_duration) {
        bush_frame = (bush_frame + 1) % 2;
        bush_frame_time = 0.0f;
    }
}

void Map::draw(float tile_size, int tiles) {
    for (int y = 0; y < map_data.size(); y++) {
        for (int x = 0; x < map_data[y].size(); x++) {
            char tile = map_data[y][x];
            Vector2 position = { x * tile_size, y * tile_size };

            if (tile == 'D') {
                DrawTextureEx(dirt, position, 0.0f, tile_size / tiles, WHITE);
            }
            else if (tile == 'P') {
                DrawTextureEx(path, position, 0.0f, tile_size / tiles, WHITE);
            }
            else if (tile == 'S') {
                DrawTextureEx(dirt_stones, position, 0.0f, tile_size / tiles, WHITE);
            }
            else if (tile == 'G') {
                DrawTextureEx(dirt_grass, position, 0.0f, tile_size / tiles, WHITE);
            }
            else if (tile == 'B') {
                int frame_width = bush_spritesheet.width / 2;
                int frame_height = bush_spritesheet.height;
                Rectangle source_rectangle = { bush_frame * frame_width, 0, frame_width, frame_height };
                Rectangle destination_rectangle = { position.x, position.y, tile_size, tile_size };
                Vector2 origin = { 0, 0 };

                DrawTexturePro(bush_spritesheet, source_rectangle, destination_rectangle, origin, 0.0f, WHITE);
            }
        }
    }
}

void Map::load_audio() {
    InitAudioDevice();
    SetMasterVolume(1.0f);

    hurt = LoadSound("audio/FX/hurt.wav");
}

void Map::unload_audio() {
    CloseAudioDevice();

    UnloadSound(hurt);
}

void Map::load_textures() {
    dirt = LoadTexture("sprites/dirt.png");
    path = LoadTexture("sprites/path.png");
    dirt_stones = LoadTexture("sprites/dirt_stones.png");
    dirt_grass = LoadTexture("sprites/dirt_grass.png");
    bush_spritesheet = LoadTexture("sprites/bush_spritesheet.png");
}

void Map::unload_textures() {
	UnloadTexture(dirt);
	UnloadTexture(path);
}

Map::~Map() {

}