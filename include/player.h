#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
public:
	Vector2 player_pos;
	Vector2 player_size;
	Texture2D player_character_spritesheet;
	Texture2D player_character_death;
};

#endif
