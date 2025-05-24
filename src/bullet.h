#pragma once
#include "entity.h"

class Bullet : public Entity {
protected:
	int damage;
	Vector2 velocity;
	int speed = 5;
public:
	Bullet(Vector2 position);

	Bullet(const Bullet&) = delete;
	Bullet& operator=(const Bullet&) = delete;
	Bullet(Bullet&&) = default;
	Bullet& operator=(Bullet&&) = default;

	void set_damage(int new_damage);
	void set_velocity(Vector2 new_velocity);
	Vector2 get_velocity();
	int get_speed();
};