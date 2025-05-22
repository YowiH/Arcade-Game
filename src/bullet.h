#pragma once
#include "entity.h"

class Bullet : public Entity {
protected:
	int damage;
	Vector2 velocity;
	int speed = 5;
public:
	Bullet(Vector2 position);
	void set_damage(int new_damage);
	void set_velocity(Vector2 new_velocity);
	Vector2 get_velocity() const;
	int get_speed() const;
};