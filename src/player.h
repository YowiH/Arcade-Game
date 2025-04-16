#pragma once

#include "raylib.h"

class Player {
private:
	int health;
	int speed;
	int coins;
	
	float tile_size;
	Vector2 size;

	float position_x;
	float position_y;
	Vector2 position;
public:
	Player();
	void draw();
	void move();
};