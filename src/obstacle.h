#pragma once
#include <raylib.h>
#include "entity.h"

class Obstacle : public Entity {
public:
	Obstacle(float x, float y, const float tile_size);
};