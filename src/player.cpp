#include "player.h"
#include "raymath.h"

Player::Player() {
	health = 3;
	coins = 0;
	
	tile_size = 32.0f;
	size = { tile_size, tile_size };
	position = { 50.0f, 50.0f };
	speed = 300;

	shoot_direction = { 0, 0 };
	fire_cooldown = 0.0f;
	fire_rate = 0.2f;
}

void Player::move(int screen_width, int screen_height) {
	float delta_time = GetFrameTime();
	if (IsKeyDown(KEY_W)) {
		position.y -= speed * delta_time;
	}
	if (IsKeyDown(KEY_S)) {
		position.y += speed * delta_time;
	}
	if (IsKeyDown(KEY_A)) {
		position.x -= speed * delta_time;
	}
	if (IsKeyDown(KEY_D)) {
		position.x += speed * delta_time;
	}
	position.x = Clamp(position.x, 0, screen_width - size.x);
	position.y = Clamp(position.y, 0, screen_height - size.y);
}

void Player::draw() {
	DrawRectangleV(position, size, WHITE);
}

Vector2 Player::get_center() const {
	return { position.x + size.x / 2.0f, position.y + size.y / 2.0f };
}

Vector2 Player::get_shoot_direction() {
	shoot_direction = { 0,0 };

	if (IsKeyDown(KEY_UP)) {
		shoot_direction = { 0, -1 };
	}
	if (IsKeyDown(KEY_DOWN)) {
		shoot_direction = { 0, 1 };
	}
	if (IsKeyDown(KEY_LEFT)) {
		shoot_direction = { -1, 0 };
	}
	if (IsKeyDown(KEY_RIGHT)) {
		shoot_direction = { 1, 0 };
	}

	return shoot_direction;
}

bool Player::can_shoot() {
	if (fire_cooldown > 0) {
		fire_cooldown -= GetFrameTime();
		return false;
	}
	return true;
}

void Player::reset_fire_cooldown() {
	fire_cooldown = fire_rate;
}