#include "player.h"
#include "raymath.h"

Player::Player(float ts) {
	tile_size = ts;

	health = 3;
	invincible = false;
	invincibility_timer = 0.0f;
	invincibility_duration = 3.0f;
	visible = true;

	coins = 0;
	
	size = { tile_size, tile_size };
	position = { 50.0f, 50.0f };

	tiles_per_second = 6.0f;
	speed = tile_size * tiles_per_second;

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
	if (visible) {
		DrawRectangleV(position, size, WHITE);
	}
}

void Player::set_position(Vector2 new_position) {
	position = new_position;

}

Vector2 Player::get_position() const {
	return position;
}

float Player::get_tile_size() const {
	return tile_size;
}

Vector2 Player::get_center() const {
	return { position.x + size.x / 2.0f, position.y + size.y / 2.0f };
}

Vector2 Player::get_shoot_direction() {
	shoot_direction = { 0,0 };

	if (IsKeyDown(KEY_UP)) {
		shoot_direction.y = -1;
	}
	if (IsKeyDown(KEY_DOWN)) {
		shoot_direction.y = 1;
	}
	if (IsKeyDown(KEY_LEFT)) {
		shoot_direction.x = -1;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		shoot_direction.x = 1;
	}

	if (shoot_direction.x != 0 && shoot_direction.y != 0) {
		shoot_direction = Vector2Normalize(shoot_direction);
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

bool Player::take_damage(int amount) {
	if (!invincible) {
		health -= amount;
		invincible = true;
		invincibility_timer = invincibility_duration;
		return true;
	}
	return false;
}

void Player::reset_invincibility() {
	invincible = false;
	invincibility_timer = 0.0f;
}

void Player::update_invincibility(float delta_time) {
	if (invincible) {
		invincibility_timer -= delta_time;
		if (invincibility_timer <= 0.0f) {
			reset_invincibility();
		}
		visible = (static_cast<int>(invincibility_timer * 10) % 2) == 0;
	}
}

void Player::reset_health() {
	health = 3;
}

int Player::get_health() const {
	return health;
}

Player::~Player() {
}