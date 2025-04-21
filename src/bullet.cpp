#include "bullet.h"
#include "raymath.h"

Bullet::Bullet(float tile_size, Vector2 position_start, Vector2 direction_start) {
	position = position_start;
	direction = direction_start;

	tiles_per_second = 12.0f;
	speed = tile_size * tiles_per_second;
	
	radius = tile_size / 8.0f;

	bullet = LoadTexture("sprites/bullet.png");
}

void Bullet::update(float delta_time) {
	position = Vector2Add(position, Vector2Scale(direction, speed * delta_time));
}

void Bullet::draw() {
	DrawRectangleLines(position.x, position.y, radius, radius, BLACK);
}

Vector2 Bullet::get_position() const {
	return position;
}

float Bullet::get_radius() const {
	return radius;
}

bool Bullet::is_off_screen(int screen_width, int screen_height) {
	return position.x < 0 || position.x > screen_width || position.y < 0 || position.y > screen_height;
}

Bullet::~Bullet() {

}