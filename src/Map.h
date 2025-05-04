#pragma once
#include "raylib.h"
#include <string>

class Map {
private:
	std::string M;
	char S;
public:
	Map(const char* F, char C);
	std :: string getStr();
	char getS();
	Map() = default;
};
