#include "entity.h"

// Entity ----------------------------
Entity::Entity(Rectangle rec, Vector2 pos, int h) {
	rectangle = rec;
	position = pos;
	health = h;
}

// Player ------------------------------
Player::Player(const float tile_size, const float area_size) : Entity({ position.x, position.y, tile_size, tile_size }, { tile_size * 3 + (area_size / 2), tile_size + (area_size * 3 / 4) }, 3) {
	size = { tile_size, tile_size };
	moving_direction = { 0, 0 };
	speed = 2;

	moving_direction_index = 0;
	walk_animation_counter = 0;
	death_animation = 0;

	coins = 0;
	damage = 1;

	right_foot = false;
	dying = false;
	xPosBlock = false;
	xNegBlock = false;
	yPosBlock = false;
	yNegBlock = false;
}

void Player::reset_health() {
	health = 3;
}

void Player::reset_position(const float tile_size, const float area_size) {
	position = { tile_size * 3 + (area_size / 2), tile_size + (area_size * 3 / 4) };
}

void Player::Update(const float tile_size, const float area_size) {
	//moving_direction_index = 0;
	if (IsKeyDown('A') && !xNegBlock) {
		//position = { position.x - speed, position.y };
		moving_direction.x = -1;
		moving_direction_index = 3;

	}
	else if (IsKeyDown('D') && !xPosBlock) {
		//position = { position.x + speed, position.y };
		moving_direction.x = 1;
		moving_direction_index = 1;

	}

	if (IsKeyDown('S') && !yPosBlock) {
		//position = { position.x, position.y + speed };
		moving_direction.y = 1;
		moving_direction_index = 2;

	}
	else if (IsKeyDown('W') && !yNegBlock) {
		//position = { position.x, position.y - speed };
		moving_direction.y = -1;
		moving_direction_index = 4;

	}

	//in order to make diagonal movements as fast as horizontal and vertical movements we must reduce the velocity in both directions
	if (moving_direction.y != 0 && moving_direction.x != 0) {
		position = { position.x + moving_direction.x * speed * 0.707f, position.y + moving_direction.y * speed * 0.707f };
	}
	else {
		position = { position.x + moving_direction.x * speed, position.y + moving_direction.y * speed };
	}

	//x axis limits
	if (position.x > area_size + (tile_size)) {
		position = { area_size + (tile_size), position.y };
	}
	else if (position.x < (tile_size * 4)) {
		position = { (tile_size * 4), position.y };
	}
	//y axis limits
	if (position.y > area_size - tile_size) {
		position = { position.x, area_size - tile_size };
	}
	else if (position.y < tile_size * 2) {
		position = { position.x, tile_size * 2 };
	}
	moving_direction = { 0, 0 };
	//restart block variables
	xPosBlock = false;
	xNegBlock = false;
	yPosBlock = false;
	yNegBlock = false;
}