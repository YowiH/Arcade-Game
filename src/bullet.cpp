#include "bullet.h"

Bullet::Bullet(Vector2 position_start, Vector2 direction_start) {
	position = position_start;
	direction = direction_start;
	speed = 400.0f;
	radius = 5.0f;
}

void Bullet::move(float delta_time) {
	position.x += direction.x * speed * delta_time;
	position.y += direction.y * speed * delta_time;
}

void Bullet::draw() {
	DrawCircleV(position, radius, RED);
}

bool Bullet::is_off_screen(int screen_width, int screen_height) {
	return position.x < 0 || position.x > screen_width || position.y < 0 || position.y > screen_height;
}