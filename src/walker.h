#pragma once
#include "entity.h"

class Walker : public Entity {
protected:
	int hp;
	int damage;
	float speed;

	bool canFly = false;
public:
	Walker(Rectangle rec);

	Walker(const Walker&) = delete;
	Walker& operator=(const Walker&) = delete;
	Walker(Walker&&) = default;
	Walker& operator=(Walker&&) = default;

	void set_hp(int new_hp);
	float get_speed();
	int get_hp();
};