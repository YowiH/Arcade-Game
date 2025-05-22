#include "powerUp.h"

PowerUp::PowerUp(Vector2 position) : Entity(rec) {
	set_position(position);
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