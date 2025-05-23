#include "obstacle.h"

Obstacle::Obstacle(float x, float y, const float tile_size) : Entity(rec) {
	rec = { x, y, tile_size, tile_size };
}