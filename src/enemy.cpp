#include "enemy.h"
#include "raymath.h"

Enemy::Enemy(float tile_size, Vector2 pos, float screen_width, float screen_height) {
	size = { tile_size, tile_size };
	position = pos;

	tiles_per_second = 2.0f;
	speed = tile_size * tiles_per_second;
	
	move_interval = 0.3f;
	move_timer = move_interval;
	
	margin = 1.0f;

	direction = { 0, 0 };

	if (position.y < margin) {
		direction = { 0.0f, 1.0f };
	}
	else if (position.y > screen_height - margin) {
		direction = { 0.0f, -1.0f };
	}
	else if (position.x < margin) {
		direction = { 1.0f, 0.0f };
	}
	else if (position.x > screen_width - margin) {
		direction = { -1.0f, 0.0f };
	}
}

bool Enemy::is_off_screen(float tile_size, int screen_width, int screen_height) const {
	return (position.x < -tile_size || position.x > screen_width + tile_size || position.y < -tile_size || position.y > screen_height + tile_size);
}

bool Enemy::is_on_grid(float tile_size) const {
	return static_cast<int>(position.x) % static_cast<int>(tile_size) == 0 && static_cast<int>(position.y) % static_cast<int>(tile_size) == 0;
}

void Enemy::move(float tile_size, Vector2 player_position, float delta_time) {
	move_timer += delta_time;

	if (!is_on_grid(tile_size)) {
		position = Vector2Add(position, Vector2Scale(direction, speed * delta_time));
		return;
	}

	position.x = round(position.x / tile_size) * tile_size;
	position.y = round(position.y / tile_size) * tile_size;

	if (move_timer >= move_interval) {
		move_timer = 0.0f;

		Vector2 difference = Vector2Subtract(player_position, position);

		if (fabs(difference.x) > fabs(difference.y)) {
			direction = { (difference.x > 0) ? 1.0f : -1.0f, 0.0f };
		}
		else {
			direction = { 0.0f, (difference.y > 0) ? 1.0f : -1.0f };
		}

		if (GetRandomValue(0, 100) < 5) {
			if (GetRandomValue(0, 1) == 0) {
				direction = { (GetRandomValue(0, 1) == 0) ? 1.0f : -1.0f, 0.0f };
			}
			else {
				direction = { 0.0f, (GetRandomValue(0, 1) == 0) ? 1.0f : -1.0f };
			}
		}
	}

	position = Vector2Add(position, Vector2Scale(direction, speed * delta_time));
}

void Enemy::draw() {
	DrawRectangleV(position, size, BLACK);
}

bool Enemy::check_collision(float tile_size, Vector2 bullet_position, float bullet_radius) {
	Rectangle enemy_rectangle = { position.x, position.y, tile_size, tile_size };
	return CheckCollisionCircleRec(bullet_position, bullet_radius, enemy_rectangle);
}

Vector2 Enemy::get_position() const {
	return position;
}

Enemy::~Enemy() {

}