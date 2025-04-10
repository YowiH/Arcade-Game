#pragma once
#include "raylib.h"
#include <string>

class Map {
private:
	std::string M;
public:
	Map(const char* F);
	std :: string getStr();


	Map() = default;
};
