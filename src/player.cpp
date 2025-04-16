#include "player.h"

Player::Player() {
	tile_size = 32.0f;
	size = { tile_size, tile_size };
	position_x = 50.0f;
	position_y = 50.0f;
	position = { position_x, position_y };
	speed = 300;
}

void Player::draw() {
	position = { position_x, position_y };
	DrawRectangleV(position, size, WHITE);
}

void Player::move() {
	float delta_time = GetFrameTime();

	if (IsKeyDown(KEY_W)) {
		position_y -= speed * delta_time;
	}
	if (IsKeyDown(KEY_S)) {
		position_y += speed * delta_time;
	}
	if (IsKeyDown(KEY_A)) {
		position_x -= speed * delta_time;
	}
	if (IsKeyDown(KEY_D)) {
		position_x += speed * delta_time;
	}
}