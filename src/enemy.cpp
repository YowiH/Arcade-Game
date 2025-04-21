#include "enemy.h"
#include "raymath.h"

#include <iostream>

Enemy::Enemy(float tile_size, Vector2 pos, float screen_width, float screen_height) {
	size = { tile_size, tile_size };
	position = pos;

	tiles_per_second = 2.0f;
	speed = tile_size * tiles_per_second;
	
	move_interval = 0.3f;
	move_timer = move_interval;
	
	margin = 1.0f;

	direction = { 0, 0 };

	enemy_frame = 0;
	enemy_frame_time = 0.0f;
	enemy_frame_duration = 0.3f;

	orc_spritesheet = LoadTexture("sprites/orc_spritesheet.png");
}

void Enemy::update(float tile_size, Vector2 player_position, float delta_time) {

	Vector2 direction = Vector2Normalize(Vector2Subtract(player_position, position));
	position = Vector2Add(position, Vector2Scale(direction, speed * delta_time));

	enemy_frame_time += delta_time;
	if (enemy_frame_time >= enemy_frame_duration) {
		enemy_frame = (enemy_frame + 1) % 2;
		enemy_frame_time = 0.0f;
	}
}

void Enemy::draw(float tile_size) {
	float frame_width = orc_spritesheet.width / 2;
	float frame_height = orc_spritesheet.height;

	Rectangle source_rectangle{ frame_width * enemy_frame, 0, frame_width, frame_height };

	Rectangle destination_rectangle = { position.x, position.y, tile_size, tile_size };
	Vector2 origin = { 0, 0 };

	DrawTexturePro(orc_spritesheet, source_rectangle, destination_rectangle, origin, 0.0f, WHITE);
}

bool Enemy::check_collision(float tile_size, Vector2 bullet_position, float bullet_radius) {
	Rectangle enemy_rectangle = { position.x, position.y, tile_size, tile_size };
	return CheckCollisionCircleRec(bullet_position, bullet_radius, enemy_rectangle);
}

bool Enemy::is_off_screen(float tile_size, int screen_width, int screen_height) const {
	return (position.x < -tile_size || position.x > screen_width + tile_size || position.y < -tile_size || position.y > screen_height + tile_size);
}

bool Enemy::is_on_grid(float tile_size) const {
	return static_cast<int>(position.x) % static_cast<int>(tile_size) == 0 && static_cast<int>(position.y) % static_cast<int>(tile_size) == 0;
}

Vector2 Enemy::get_position() const {
	return position;
}

Enemy::~Enemy() {

}