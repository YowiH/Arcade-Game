#pragma once

#include "raylib.h"
#include <vector>

#include "bullet.h"

class Player {
private:
	int health;
	int speed;
	int coins;
	
	float tile_size;
	Vector2 size;
	Vector2 position;

	Vector2 shoot_direction;
	float fire_cooldown;
	float fire_rate;
public:
	Player();

	void move(int screen_width, int screen_height);
	void draw();

	Vector2 get_center() const;
	Vector2 get_shoot_direction();
	bool can_shoot();
	void reset_fire_cooldown();
};