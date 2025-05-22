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
	void set_hp(int new_hp);
	float get_speed() const;
	int get_hp() const;
};