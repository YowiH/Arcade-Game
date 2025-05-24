#pragma once
#include "walker.h"

class Enemy : public Walker {
protected:
	int animation_counter = 0;
	Vector2 velocity;
	bool right_foot;
public:
	Enemy(Vector2 pos);

	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;
	Enemy(Enemy&&) = default;
	Enemy& operator=(Enemy&&) = default;

	bool get_right_foot();
	int get_animation_counter();

	void add_animation_counter(int amount);
	void set_animation_counter(int new_amount);
	void set_right_foot(bool new_state);
};