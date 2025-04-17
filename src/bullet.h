#pragma once

#include "raylib.h"

class Bullet {
public:
	Vector2 position;
	Vector2 direction;
	float speed;
	float radius;

	Bullet(Vector2 position_start, Vector2 direction_start);
	void move(float delta_time);
	void draw();
	bool is_off_screen(int screen_width, int screen_height);
};