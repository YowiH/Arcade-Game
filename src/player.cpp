#include "player.h"
#include "raymath.h"

Player::Player(float tile_size, float sw, float sh) {

	size = { tile_size, tile_size };
	position = { sw / 2, sh / 2};

	health = 3;
	coins = 0;
	tiles_per_second = 3.0f;
	speed = tile_size * tiles_per_second;

	visible = true;
	invincible = false;
	invincibility_timer = 0.0f;
	invincibility_duration = 3.0f;
	
	animation_timer = 0.0f;
	animation_speed = 0.2f;

	frame_columns = 7;
	frame_rows = 2;
	frame_width = 16.0f;
	frame_height = 16.0f;
	frame_index = 0;
	frame_rectangle = { 0 };
	player_spritesheet = { 0 };

	shoot_direction = { 0, 0 };
	fire_cooldown = 0.0f;
	fire_rate = 0.2f;

	current_animation = player_animation::IDLE;
	last_direction = player_animation::IDLE;
}


bool Player::is_moving() {
	return IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D);
}

void Player::move(int screen_width, int screen_height) {
	float delta_time = GetFrameTime();

	if (!is_moving()) {
		if (is_shooting()) {
			if (shoot_direction.y == -1) {
				set_animation(player_animation::UP);
			}
			else if (shoot_direction.y == 1) {
				set_animation(player_animation::DOWN);
			}
			if (shoot_direction.x == -1) {
				set_animation(player_animation::LEFT);
			}
			else if (shoot_direction.x == 1) {
				set_animation(player_animation::RIGHT);
			}
		}
		else {
			set_animation(player_animation::IDLE);
			last_direction = player_animation::IDLE;
		}
	}

	if (IsKeyDown(KEY_W)) {
		if (last_direction != player_animation::UP_M1 && last_direction != player_animation::UP_M2) {
			set_animation(player_animation::UP_M1);
			last_direction = player_animation::UP_M1;
		}
		position.y -= speed * delta_time;
	}
	if (IsKeyDown(KEY_S)) {
		if (last_direction != player_animation::DOWN_M1 && last_direction != player_animation::DOWN_M2) {
			set_animation(player_animation::DOWN_M1);
			last_direction = player_animation::DOWN_M1;
		}
		position.y += speed * delta_time;
	}
	if (IsKeyDown(KEY_A)) {
		if (last_direction != player_animation::LEFT_M1 && last_direction != player_animation::LEFT_M2) {
			set_animation(player_animation::LEFT_M1);
			last_direction = player_animation::LEFT_M1;
		}
		position.x -= speed * delta_time;
	}
	if (IsKeyDown(KEY_D)) {
		if (last_direction != player_animation::RIGHT_M1 && last_direction != player_animation::RIGHT_M2) {
			set_animation(player_animation::RIGHT_M1);
			last_direction = player_animation::RIGHT_M1;
		}
		position.x += speed * delta_time;
	}

	position.x = Clamp(position.x, 0, screen_width - size.x);
	position.y = Clamp(position.y, 0, screen_height - size.y);
}

void Player::set_animation(player_animation animation) {
	current_animation = animation;
}

void Player::update_texture(float delta_time) {
	if (is_moving()) {
		animation_timer += delta_time;

		if (animation_timer >= animation_speed) {
			switch (current_animation) {
				case player_animation::UP_M1:
					set_animation(player_animation::UP_M2);
					break;
				case player_animation::UP_M2:
					set_animation(player_animation::UP_M1);
					break;
				case player_animation::DOWN_M1:
					set_animation(player_animation::DOWN_M2);
					break;
				case player_animation::DOWN_M2:
					set_animation(player_animation::DOWN_M1);
					break;
				case player_animation::LEFT_M1:
					set_animation(player_animation::LEFT_M2);
					break;
				case player_animation::LEFT_M2:
					set_animation(player_animation::LEFT_M1);
					break;
				case player_animation::RIGHT_M1:
					set_animation(player_animation::RIGHT_M2);
					break;
				case player_animation::RIGHT_M2:
					set_animation(player_animation::RIGHT_M1);
					break;
				default:
					break;
			}
			animation_timer = 0.0f;
		}
	}

	int anim_index = static_cast<int>(current_animation);
	int column = anim_index % frame_columns;
	int row = anim_index / frame_columns;
	
	frame_rectangle.x = column * frame_width;
	frame_rectangle.y = row * frame_height;
	frame_rectangle.width = frame_width;
	frame_rectangle.height = frame_height;
}

void Player::draw(float tile_size) {
	if (visible) {
		Rectangle dest_rectangle = { position.x, position.y, tile_size, tile_size };
		Vector2 origin = { 0,0 };
		DrawTexturePro(player_spritesheet, frame_rectangle, dest_rectangle, origin, 0.0f, WHITE);
	}
}

void Player::set_position(Vector2 new_position) {
	position = new_position;
}

Vector2 Player::get_position() const {
	return position;
}

Vector2 Player::get_center() const {
	return { position.x + size.x / 2.0f, position.y + size.y / 2.0f };
}

bool Player::is_shooting() {
	return IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT);
}

Vector2 Player::get_shoot_direction() {
	shoot_direction = { 0,0 };

	if (IsKeyDown(KEY_UP)) {
		shoot_direction.y = -1;
	}
	if (IsKeyDown(KEY_DOWN)) {
		shoot_direction.y = 1;
	}
	if (IsKeyDown(KEY_LEFT)) {
		shoot_direction.x = -1;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		shoot_direction.x = 1;
	}

	if (shoot_direction.x != 0 && shoot_direction.y != 0) {
		shoot_direction = Vector2Normalize(shoot_direction);
	}

	return shoot_direction;
}

bool Player::can_shoot() {
	if (fire_cooldown > 0) {
		fire_cooldown -= GetFrameTime();
		return false;
	}
	return true;
}

void Player::reset_fire_cooldown() {
	fire_cooldown = fire_rate;
}

bool Player::take_damage(int amount) {
	if (!invincible) {
		health -= amount;
		invincible = true;
		invincibility_timer = invincibility_duration;
		return true;
	}
	return false;
}

void Player::reset_invincibility() {
	invincible = false;
	invincibility_timer = 0.0f;
}

void Player::update_invincibility(float delta_time) {
	if (invincible) {
		invincibility_timer -= delta_time;
		if (invincibility_timer <= 0.0f) {
			reset_invincibility();
		}
		visible = (static_cast<int>(invincibility_timer * 10) % 2) == 0;
	}
}


void Player::reset_health() {
	health = 3;
}

int Player::get_health() const {
	return health;
}

void Player::load_texture() {
	player_spritesheet = LoadTexture("sprites/player_spritesheet.png");
}

Player::~Player() {
}