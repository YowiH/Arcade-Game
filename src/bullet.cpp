#include "bullet.h"

Bullet::Bullet(Vector2 position) : Entity({position.x, position.y, tile_size, tile_size}) {
	damage = 1;
	velocity = { 0, 0 };
}

void Bullet::set_damage(int new_damage) {
	damage = new_damage;
}

void Bullet::set_velocity(Vector2 new_velocity) {
	velocity = new_velocity;
}

Vector2 Bullet::get_velocity() {
	return velocity;
}

int Bullet::get_speed() {
	return speed;
}