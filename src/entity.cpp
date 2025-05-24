#include "entity.h"

Entity::Entity(Rectangle rec) {
	this->rec = rec;
}

Rectangle Entity::get_rec() {
	return rec;
}

void Entity::set_rec(Rectangle& new_rec) {
	rec = new_rec;
}

Vector2 Entity::get_position() {
	return { rec.x, rec.y };
}

void Entity::set_position(Vector2 position) {
	rec = { position.x, position.y, tile_size, tile_size };
}