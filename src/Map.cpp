#include "Map.h"
Map::Map(const char* F, char C) {
	M = LoadFileText(F);
	S = C;
}
std::string Map::getStr()
{
	return M;
}
char Map::getS() {
	return S;
}
