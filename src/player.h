#pragma once

#include "raylib.h"
#include <vector>
#include <string>

#include "bullet.h"

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
	int health;
	bool invincible;
	float invincibility_timer;
	float invincibility_duration;
	bool visible;

	int coins;
	
	Vector2 size;
	Vector2 position;
	int speed;
	int tiles_per_second;

	float animation_timer;
	float animation_speed;

	Vector2 shoot_direction;
	float fire_cooldown;
	float fire_rate;

	player_animation current_animation;
	player_animation last_direction;

	float frame_width;
	float frame_height;

	int frame_columns;
	int frame_rows;
	int frame_index;

	Rectangle frame_rectangle;
	Texture2D player_sprite_sheet;
public:
	Player(float ts, float sw, float sh);

	bool is_moving();
	void move(int screen_width, int screen_height);
	void set_animation(player_animation animation);
	void draw(float tile_size);

	void set_position(Vector2 new_position);
	Vector2 get_position() const;
	Vector2 get_center() const;

	bool is_shooting();
	Vector2 get_shoot_direction();
	bool can_shoot();
	void reset_fire_cooldown();

	bool take_damage(int amount);
	void reset_invincibility();
	void update_invincibility(float delta_time);
	void update_texture(float delta_time);
	void reset_health();
	int get_health() const;

	void load_texture();

	~Player();
};