#pragma once

#include "raylib.h"

class Enemy {
private:
	Vector2 position;
	Vector2 size;
	int tiles_per_second;
	float speed;
	
	float margin;
	float move_timer;
	float move_interval;
	Vector2 direction;
public:
	Enemy(float tile_size, Vector2 pos, float sw, float sh);

	bool is_off_screen(float tile_size, int screen_width, int screen_height) const;
	bool is_on_grid(float tile_size) const;
	void move(float tile_size, Vector2 player_position, float delta_time);
	void draw();
	bool check_collision(float tile_size, Vector2 bullet_position, float bullet_radius);
	
	Vector2 get_position() const;

	~Enemy();
};