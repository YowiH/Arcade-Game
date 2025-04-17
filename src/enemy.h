#pragma once

#include "raylib.h"

class Enemy {
private:
	Vector2 position;
	float speed;
	float size;
public:
	Enemy(Vector2 pos, float spd);

	void move(Vector2 player_position, float delta_time);
	void draw();
	bool is_off_screen(int screen_width, int screen_height) const;
	bool check_collision(Vector2 bullet_position, float bullet_radius);
	
	Vector2 get_position() const;
	float get_size() const;
};