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
};