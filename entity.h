#pragma once
#include <raylib.h>
#include <vector>

class Entity_Manager {
	std::vector<Entity> active{}; // tracker
	std::vector<Entity> archived{}; // pool
};

// General --------------------------
class Entity {
protected:
	Rectangle rectangle;
	Vector2 position;
	int health;
public:
	Entity(Rectangle rec, Vector2 pos, int h);
	virtual void Update() {};
	virtual void Draw() {};
};

// Player ------------------------------
class Player : public Entity {
private:
	Vector2 size;
	Vector2 moving_direction;
	float speed;

	int moving_direction_index;
	int walk_animation_counter;
	int death_animation;

	int coins;
	int damage;

	bool right_foot;
	bool dying;
	bool xPosBlock;
	bool xNegBlock;
	bool yPosBlock;
	bool yNegBlock;
public:
	Player(const float tile_size, const float area_size);
	void reset_health();
	void reset_position(const float tile_size, const float area_size);
	void Update(const float tile_size, const float area_size);
	void Draw() override;
};

// Enemy -------------------------------
class Enemy : public Entity {

};

class Orc : public Enemy {
	Texture2D orc_spritesheet;
};

// Boss --------------------------------
class Boss : public Entity {

};

// Item ---------------------------------
class Item : public Entity {

};