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
	Rectangle get_rec() const;
	void set_rec(const Rectangle& new_rec);
	void set_position(Vector2 position);
};