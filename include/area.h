#pragma once

#include "raylib.h"

class Area {
public:
    bool wall_frame;

    //general terrain
    Texture2D bridge;

    //desert terrain
    Texture2D dirt_grass;
    Texture2D dirt;
    Texture2D dirt_stones;
    Texture2D path;
    Texture2D path_stones;
    Texture2D river_desert;
    Texture2D bush_spritesheet;
    Texture2D logs;

    //forest terrain
    Texture2D grass;
    Texture2D flowers_grass;
    Texture2D path_grass;
    Texture2D path_grass2;
    Texture2D tall_grass;
    Texture2D river_forest;
    Texture2D green_bush_spritesheet;
    Texture2D stump;

    //cementery terrain
    Texture2D cementery_floor;
    Texture2D cementery_gravel;
    Texture2D cementery_path;
    Texture2D stone_floor;
    Texture2D tombstone;

    void loadAssets();
}