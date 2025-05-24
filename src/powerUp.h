#pragma once
#include "entity.h"

class PowerUp : public Entity {
protected:
	char type;
	int despawn_timer = 0;
public:
	PowerUp(Vector2 position);

	PowerUp(const PowerUp&) = delete;
	PowerUp& operator=(const PowerUp&) = delete;
	PowerUp(PowerUp&&) = default;
	PowerUp& operator=(PowerUp&&) = default;


	int get_despawn_timer();
	char get_type();

	void set_despawn_timer(int new_despawn_timer);
	void add_despawn_timer(int adding_amount);
	void set_type(char new_type);
};