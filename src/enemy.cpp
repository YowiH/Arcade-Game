#include "enemy.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, float spd) {
	position = pos;
	speed = spd;
	size = 28.0f;
}

void Enemy::move(Vector2 player_position, float delta_time) {
	Vector2 direction = Vector2Normalize(Vector2Subtract(player_position, position));
	position = Vector2Add(position, Vector2Scale(direction, speed * delta_time));
}

void Enemy::draw() {
	DrawRectangleV(position, { size, size }, BLUE);
}

bool Enemy::is_off_screen(int screen_width, int screen_height) const {
	return (position.x < -size || position.x > screen_width + size || position.y < -size || position.y > screen_height + size);
}

bool Enemy::check_collision(Vector2 bullet_position, float bullet_radius) {
	Rectangle enemy_rectangle = { position.x, position.y, size, size };
	return CheckCollisionCircleRec(bullet_position, bullet_radius, enemy_rectangle);
}

Vector2 Enemy::get_position() const {
	return position;
}

float Enemy::get_size() const {
	return size;
}