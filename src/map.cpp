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

void Map::load(float tile_size) {

    // load map
    std::ifstream map_file("areas/1-1.txt");
    if (!map_file.is_open()) {
        TraceLog(LOG_ERROR, "Failed to load map file");
        return;
    }

    std::string line;
    int y = 0;

    std::vector<Rectangle> bushes;

    while (std::getline(map_file, line)) {
        std::string cleaned;
        int x = 0;

        for (char c : line) {
            if (c != ' ') {
                cleaned += c;

                if (c == 'B') {
                    Rectangle solid = { x * tile_size, y * tile_size, tile_size, tile_size };
                    bushes.push_back(solid);
                }
                ++x;
            }
        }

        if (!cleaned.empty()) {
            map_data.push_back(cleaned);
            ++y;
        }
    }

    this->bushes = bushes;

    // load music
    music = LoadMusicStream("audio/songs/JOTPK_song.wav");
    PlayMusicStream(music);

    // load audio
    hurt = LoadSound("audio/FX/death_player.mp3");
    shoot_1 = LoadSound("audio/FX/shoot_1.mp3");
    shoot_2 = LoadSound("audio/FX/shoot_2.mp3");
    shoot_3 = LoadSound("audio/FX/shoot_3.mp3");
    shoot_4 = LoadSound("audio/FX/shoot_4.mp3");
    shoot_5 = LoadSound("audio/FX/shoot_5.mp3");
    shoot_6 = LoadSound("audio/FX/shoot_6.mp3");
    shoot_7 = LoadSound("audio/FX/shoot_7.mp3");
    enemy_death_1 = LoadSound("audio/FX/enemy_death_1.mp3");
    enemy_death_2 = LoadSound("audio/FX/enemy_death_2.mp3");
    enemy_death_3 = LoadSound("audio/FX/enemy_death_3.mp3");
    enemy_death_4 = LoadSound("audio/FX/enemy_death_4.mp3");


    // load textures
    dirt = LoadTexture("sprites/dirt.png");
    path = LoadTexture("sprites/path.png");
    dirt_stones = LoadTexture("sprites/dirt_stones.png");
    dirt_grass = LoadTexture("sprites/dirt_grass.png");
    bush_spritesheet = LoadTexture("sprites/bush_spritesheet.png");
}

void Map::update(float delta_time) {
    UpdateMusicStream(music);

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

Sound Map::get_hurt() const {
    return hurt;
}

Sound Map::get_shoot() const {
    switch (GetRandomValue(1, 7)) {
    case 1:
        return shoot_1;
        break;
    case 2:
        return shoot_2;
        break;
    case 3:
        return shoot_3;
        break;
    case 4:
        return shoot_4;
        break;
    case 5:
        return shoot_5;
        break;
    case 6:
        return shoot_6;
        break;
    case 7:
        return shoot_7;
        break;
    }
}

Sound Map::get_enemy_death() const {
    switch (GetRandomValue(1, 4)) {
    case 1:
        return enemy_death_1;
        break;
    case 2:
        return enemy_death_2;
        break;
    case 3:
        return enemy_death_3;
        break;
    case 4:
        return enemy_death_4;
        break;
    }
}

bool Map::check_collision(Rectangle player_rectangle) {
    for (const Rectangle& bush : bushes) {
        if (CheckCollisionRecs(player_rectangle, bush)) {
            return true;
        }
    }
    return false;
}

Map::~Map() {
    // unload map
    map_data.clear();

    // unload audio
    UnloadMusicStream(music);
    UnloadSound(hurt);
    UnloadSound(shoot_1);
    UnloadSound(shoot_2);
    UnloadSound(shoot_3);
    UnloadSound(shoot_4);
    UnloadSound(shoot_5);
    UnloadSound(shoot_6);
    UnloadSound(shoot_7);
    UnloadSound(enemy_death_1);
    UnloadSound(enemy_death_2);
    UnloadSound(enemy_death_3);
    UnloadSound(enemy_death_4);
    CloseAudioDevice();

    // unload textures
    UnloadTexture(dirt);
    UnloadTexture(path);
    UnloadTexture(dirt_stones);
    UnloadTexture(dirt_grass);
    UnloadTexture(bush_spritesheet);
}