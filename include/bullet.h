#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

class Bullet {
public:
    int speed;
    Vector2 velocity;
    Vector2 position;
    int damage;
    void loadAssets();
};

#endif