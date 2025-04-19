#pragma once

#include "raylib.h"
#include <vector>

#include "bullet.h"

class Player {
private:
	int health;
	bool invincible;
	float invincibility_timer;
	float invincibility_duration;
	bool visible;

	int coins;
	
	float tile_size;
	Vector2 size;
	Vector2 position;
	int speed;
	int tiles_per_second;

	Vector2 shoot_direction;
	float fire_cooldown;
	float fire_rate;

public:
	Player(float ts);

	void move(int screen_width, int screen_height);
	void draw();

	Vector2 get_position() const;
	float get_tile_size() const;
	Vector2 get_center() const;

	Vector2 get_shoot_direction();
	bool can_shoot();
	void reset_fire_cooldown();

	bool take_damage(int amount);
	void reset_invincibility();
	void update_invincibility(float delta_time);
	int get_health() const;

	~Player();
};