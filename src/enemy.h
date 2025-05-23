#pragma once
#include "walker.h"

class Enemy : public Walker {
protected:
	int animation_counter = 0;
	Vector2 velocity;
	bool right_foot;
public:
	Enemy(Vector2 pos);

	bool get_right_foot() const;
	int get_animation_counter() const;

	void add_animation_counter(int amount);

	void set_animation_counter(int new_amount);
	void set_right_foot(bool new_state);
};