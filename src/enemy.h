#pragma once

#include "raylib.h"

class Enemy {
private:
	Vector2 position;
	float tile_size;
	Vector2 size;
	int tiles_per_second;
	float speed;
public:
	Enemy(float ts, Vector2 pos);

	void move(Vector2 player_position, float delta_time);
	void draw();
	bool is_off_screen(int screen_width, int screen_height) const;
	bool check_collision(Vector2 bullet_position, float bullet_radius);
	
	Vector2 get_position() const;
	float get_size() const;
};