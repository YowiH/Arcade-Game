#include "enemy.h"
#include "player.h"

Enemy::Enemy(Vector2 pos) : Walker({ pos.x, pos.y, tile_size, tile_size }) {
	hp = 1;
	damage = 1;
	speed = 1.25;
	right_foot = false;
	velocity = { 0, 0 };
}

bool Enemy::get_right_foot() {
	return right_foot;
}

int Enemy::get_animation_counter() {
	return animation_counter;
}

void Enemy::add_animation_counter(int amount) {
	animation_counter += amount;
}

void Enemy::set_animation_counter(int new_amount) {
	animation_counter = new_amount;
}

void Enemy::set_right_foot(bool new_state) {
	right_foot = new_state;
}