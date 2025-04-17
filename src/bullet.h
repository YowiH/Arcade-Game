#pragma once

#include "raylib.h"

class Bullet {
private:
	Vector2 position;
	Vector2 direction;
	float speed;
	float radius;
public:
	Bullet(Vector2 position_start, Vector2 direction_start);
	void move(float delta_time);
	void draw();
	Vector2 get_position() const;
	float get_radius() const;
	bool is_off_screen(int screen_width, int screen_height);
};