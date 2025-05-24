#pragma once
#include <raylib.h>
#include "entity.h"

class Obstacle : public Entity {
private:
	char type; // 'V' for vegetation, 'O' for obstacle, 'B' for bridge, etc.
public:
	Obstacle(float x, float y, const float tile_size, char t);
	char get_type() const { return type; };
};