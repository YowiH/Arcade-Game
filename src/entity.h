#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>

class Entity {
protected:
	Rectangle rec;
	const float tile_size = 32;
	const float left_margin = tile_size * 3;
	const float area_size = tile_size * 16;
public:
	Entity(Rectangle rec);

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity(Entity&&) = default;
	Entity& operator=(Entity&&) = default;

	Rectangle get_rec();
	void set_rec(Rectangle& new_rec);
	Vector2 get_position();
	void set_position(Vector2 position);
};