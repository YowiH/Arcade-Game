#pragma once

#include "raylib.h"

class Bullet {
private:
	Vector2 position;
	Vector2 direction;

	float radius;
	float tiles_per_second;
	float speed;

	Texture2D bullet;
public:
	Bullet(float tile_size, Vector2 position, Vector2 direction);

	void update(float delta_time);
	void draw();

	bool is_off_screen(int screen_width, int screen_height);
	
	Vector2 get_position() const;
	float get_radius() const;

	~Bullet();
};