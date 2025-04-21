#pragma once

#include "raylib.h"
#include <vector>
#include <string>

#include "bullet.h"
#include "map.h"

enum class player_animation {
	IDLE,
	USING_ITEM,
	DOWN,
	DOWN_M1,
	DOWN_M2,
	RIGHT,
	RIGHT_M1,
	RIGHT_M2,
	UP,
	UP_M1,
	UP_M2,
	LEFT,
	LEFT_M1,
	LEFT_M2
};

class Player {
private:
	// taking damage
	int health;
	bool invincible;
	float invincibility_timer;
	float invincibility_duration;
	bool visible;

	// picking up
	int coins;

	// moving
	Vector2 size;
	Vector2 position;
	int speed;
	int tiles_per_second;

	// texturing
	Rectangle source_rectangle;
	Texture2D player_spritesheet;

	float animation_timer;
	float animation_speed;
	player_animation current_animation;
	player_animation last_direction;
	float frame_width;
	float frame_height;
	int frame_columns;
	int frame_rows;
	int frame_index;

	// shooting
	Vector2 shoot_direction;
	float fire_cooldown;
	float fire_rate;

public:
	Player(float ts, float sw, float sh);

	void load();
	void update(float tile_size, float delta_time, int screen_width, int screen_height, Map& map);
	void draw(float tile_size);

	int get_health() const;
	Vector2 get_position() const;
	Vector2 get_center() const;
	Vector2 get_shoot_direction();

	void set_animation(player_animation animation);
	void set_position(Vector2 new_position);
	bool set_damage(int amount);

	void reset_fire_cooldown();
	void reset_invincibility();
	void reset_health();

	bool is_moving();
	bool is_shooting();
	bool can_shoot();

	~Player();
};