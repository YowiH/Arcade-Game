#include "Map.h"
Map::Map(const char* F) {
	M = LoadFileText(F);
}