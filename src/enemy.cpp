#include "enemy.h"
#include "player.h"

Enemy::Enemy(Vector2 pos) : Walker(rec) {
	rec = { pos.x, pos.y, tile_size, tile_size };
	hp = 1;
	damage = 1;
	speed = 1.25;
}

bool Enemy::get_right_foot() const {
	return right_foot;
}