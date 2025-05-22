#pragma once
#include "entity.h"

class PowerUp : public Entity {
protected:
	char type;
	int despawn_timer = 0;
public:
	PowerUp(Vector2 position);
	int get_despawn_timer() const;
	void set_despawn_timer(int new_despawn_timer);
	void add_despawn_timer(int adding_amount);
	void set_type(char new_type);
};