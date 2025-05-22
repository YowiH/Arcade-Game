#include "bullet.h"

Bullet::Bullet(Vector2 position) : Entity(rec) {
	set_position(position);
}

void Bullet::set_damage(int new_damage) {
	damage = new_damage;
}

void Bullet::set_velocity(Vector2 new_velocity) {
	velocity = new_velocity;
}

Vector2 Bullet::get_velocity() const {
	return velocity;
}

int Bullet::get_speed() const {
	return speed;
}