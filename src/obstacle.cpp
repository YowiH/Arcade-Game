#include "obstacle.h"

Obstacle::Obstacle(float x, float y, const float tile_size) {
	rec = { x, y, tile_size, tile_size };
}
Rectangle Obstacle::get_rec() const {
	return rec;
}