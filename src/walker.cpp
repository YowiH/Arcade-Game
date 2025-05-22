#include "walker.h"

Walker::Walker(Rectangle rec) : Entity(rec) {

}

void Walker::set_hp(int new_hp) {
	hp = new_hp;
}

float Walker::get_speed() const {
	return speed;
}

int Walker::get_hp() const {
	return hp;
}