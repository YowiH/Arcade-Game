#include "entity.h"

Entity::Entity(Rectangle rec) {
}

Rectangle Entity::get_rec() const {
	return rec;
}

void Entity::set_rec(const Rectangle& new_rec) {
	rec = new_rec;
}

Vector2 Entity::get_position() const {
	return { rec.x, rec.y };
}

void Entity::set_position(Vector2 position) {
	rec = { position.x, position.y, tile_size, tile_size };
}