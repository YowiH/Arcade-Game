#include "obstacle.h"

Obstacle::Obstacle(float x, float y) : Entity(rec) {
	rec = { x, y, tile_size, tile_size };
}