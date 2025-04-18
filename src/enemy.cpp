#include "enemy.h"
#include "raymath.h"

Enemy::Enemy(float ts, Vector2 pos) {
	tile_size = ts;
	size = { tile_size, tile_size };
	position = pos;
	tiles_per_second = 3.5f;
	speed = tile_size * tiles_per_second;
}

void Enemy::move(Vector2 player_position, float delta_time) {
	Vector2 direction = Vector2Normalize(Vector2Subtract(player_position, position));
	position = Vector2Add(position, Vector2Scale(direction, speed * delta_time));
}

void Enemy::draw() {
	DrawRectangleV(position, size, BLACK);
}

bool Enemy::is_off_screen(int screen_width, int screen_height) const {
	return (position.x < -tile_size || position.x > screen_width + tile_size || position.y < -tile_size || position.y > screen_height + tile_size);
}

bool Enemy::check_collision(Vector2 bullet_position, float bullet_radius) {
	Rectangle enemy_rectangle = { position.x, position.y, tile_size, tile_size };
	return CheckCollisionCircleRec(bullet_position, bullet_radius, enemy_rectangle);
}

Vector2 Enemy::get_position() const {
	return position;
}

float Enemy::get_size() const {
	return tile_size;
}