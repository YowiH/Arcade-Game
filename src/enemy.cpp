#include "enemy.h"
#include "player.h"

Enemy::Enemy(Vector2 pos) : Walker(rec) {
	rec = { pos.x, pos.y, tile_size, tile_size };
	hp = 1;
	damage = 1;
	speed = 1.25;
}

bool Enemy::get_right_foot() const {
	return right_foot;
}

int Enemy::get_animation_counter() const {
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