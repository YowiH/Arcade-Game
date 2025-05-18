#pragma once
#include <raylib.h>

class Obstacle {
private:
	Rectangle rec;
public:
	Obstacle(float x, float y, const float tile_size);
	Rectangle get_rec() const;
};