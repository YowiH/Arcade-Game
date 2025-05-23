#include "powerUp.h"

PowerUp::PowerUp(Vector2 position) : Entity(rec) {
	set_position(position);
}

int PowerUp::get_despawn_timer() const {
	return despawn_timer;
}

char PowerUp::get_type() const {
	return type;
}

void PowerUp::add_despawn_timer(int adding_amount) {
	despawn_timer += adding_amount;
}

void PowerUp::set_despawn_timer(int new_despawn_timer) {
	despawn_timer = new_despawn_timer;
}

void PowerUp::set_type(char new_type) {
	type = new_type;
}