#pragma once
#include <raylib.h>

class Entity {
protected:
	Rectangle rec;
public:
	Entity(Rectangle rec);
	Rectangle get_rec() const;
};