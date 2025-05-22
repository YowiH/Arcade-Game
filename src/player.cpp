#include "player.h"
#include "enemy.h"
#include "obstacle.h"
#include "powerUp.h"

Player::Player() : Walker(rec) {
	set_position({ left_margin + (area_size / 2), tile_size + (area_size * 3 / 4) });
	hp = 3;
	speed = 2;
	damage = 1;
}

void Player::move() {
	if (IsKeyDown('A') && !xNegBlock) {
		//player_pos = { player_pos.x - player_Speed, player_pos.y };
		player_mov_dir.x = -1;
		Mov_dir = 3;

	}
	else if (IsKeyDown('D') && !xPosBlock) {
		//player_pos = { player_pos.x + player_Speed, player_pos.y };
		player_mov_dir.x = 1;
		Mov_dir = 1;

	}

	if (IsKeyDown('S') && !yPosBlock) {
		//player_pos = { player_pos.x, player_pos.y + player_Speed };
		player_mov_dir.y = 1;
		Mov_dir = 2;

	}
	else if (IsKeyDown('W') && !yNegBlock) {
		//player_pos = { player_pos.x, player_pos.y - player_Speed };
		player_mov_dir.y = -1;
		Mov_dir = 4;

	}

	//in order to make diagonal movements as fast as horizontal and vertical movements we must reduce the velocity in both directions
	if (player_mov_dir.y != 0 && player_mov_dir.x != 0) {
		set_position({ rec.x + player_mov_dir.x * speed * 0.707f, rec.y + player_mov_dir.y * speed * 0.707f });
	}
	else {
		set_position({rec.x + player_mov_dir.x * speed, rec.y + player_mov_dir.y * speed});
	}

	//x axis limits
	if (rec.x > area_size + (tile_size)) {
		set_position({ area_size + (tile_size), rec.y });
	}
	else if (rec.x < (tile_size * 4)) {
		set_position({ (tile_size * 4), rec.y });
	}
	//y axis limits
	if (rec.y > area_size - tile_size) {
		set_position({ rec.x, area_size - tile_size });
	}
	else if (rec.y < tile_size * 2) {
		set_position({ rec.x, tile_size * 2 });
	}
	player_mov_dir = { 0, 0 };
	//restart block variables
	xPosBlock = false;
	xNegBlock = false;
	yPosBlock = false;
	yNegBlock = false;
}

void Player::die(Sound player_death, std::vector<Enemy>& enemy_tracker, std::vector<Enemy>& enemy_pool, int& active_enemies, std::vector<PowerUp>& powerUp_tracker, std::vector<PowerUp>& powerUp_pool) {
	player_dying = true;
	player_death_anim = 0;
	PlaySound(player_death);
	//borrar enemics
	for (int i = enemy_tracker.size() - 1; i >= 0; i--) {
		//save the bullet in the pool
		enemy_pool.push_back(enemy_tracker[i]);
		//borrar bullet
		auto& j = enemy_tracker.begin() + i;
		enemy_tracker.erase(j);

	}
	active_enemies = 0;
	//borrar totes les bales
	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
		//save the bullet in the pool
		bullet_pool.push_back(bullet_tracker[i]);
		//borrar bullet
		auto& j = bullet_tracker.begin() + i;
		bullet_tracker.erase(j);

	}
	//borrar tots els power ups
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
		//save the bullet in the pool
		powerUp_pool.push_back(powerUp_tracker[i]);
		//borrar bullet
		auto& j = powerUp_tracker.begin() + i;
		powerUp_tracker.erase(j);

	}
}

void Player::be_attacked(std::vector<Enemy>& enemy_tracker) {
	bool already_damaged = false;
	int i = 0;
	while (!already_damaged && i < enemy_tracker.size()) {
		if (CheckCollisionCircles({ rec.x + (rec.x / 2), rec.y + (rec.height / 2) }, rec.width / 2, { enemy_tracker[i].get_rec().x + (tile_size / 2), enemy_tracker[i].get_rec().y + (tile_size / 2) }, tile_size / 2)) {
			already_damaged = true;
			hp--;

			//player death
			die();

			if (hp < 0) {
				std::cout << "you are dead" << std::endl;
			}
		}
		i++;
	}
}

float GetAngle(Vector2 v, Vector2 u) {
	float c = (u.x * v.x + u.y * v.y); //prop al cosinus
	float s = (u.x * v.y - u.y * v.x); //prop al sinus

	return atan2(s, c) + PI;
}

void Player::be_restrained(float x, float y) {

	//define two vectors and use them to conclude the direction of the colision
	Vector2 v = { 0, 0 };
	Vector2 u = { 0, 0 };

	//inici vector
	float x0 = rec.x;
	float y0 = rec.y;

	//v va des del centre del obstacle (cantonada esquerra de dalt) al centre del jugador
	v = { x0 - x, y0 - y };
	//u va des del centre del jugador al centre de l'obstacle
	u = { tile_size / 2, 0 };
	float theta = GetAngle(u, v);

	if (theta >= PI / 4 && theta < 3 * PI / 4) {
		//colisio per dalt
		yNegBlock = true;
	}
	else if (theta >= 3 * PI / 4 && theta < 5 * PI / 4) {
		//colisió per l'esquerra
		xNegBlock = true;
	}
	else if (theta >= 5 * PI / 4 && theta < 7 * PI / 4) {
		//colisio per baix
		yPosBlock = true;
	}
	else if (theta >= 7 * PI / 4 || theta < PI / 4) {
		//colisio per la dreta
		xPosBlock = true;
	}
	else {
		std::cout << "error when calculating the angle" << std::endl;
	}

	return;

}

void Player::collide(std::vector<Obstacle>& obstacle_tracker) {
	//check all obstacle colliders
	Rectangle rec_player = { rec.x, rec.y, tile_size, tile_size };
	for (int i = 0; i < obstacle_tracker.size(); i++) {
		if (CheckCollisionRecs(rec_player, obstacle_tracker[i].get_rec())) {
			be_restrained(obstacle_tracker[i].get_rec().x, obstacle_tracker[i].get_rec().y);
		}
	}
}


void collect() {
	for (int i = powerUp_tracker.size() - 1; i >= 0; i--) {
		if (CheckCollisionCircles({ player_pos.x + (player_size.x / 2), player_pos.y + (player_size.y / 2) }, player_size.x / 2, { powerUp_tracker[i].position.x + tile_size / 2, powerUp_tracker[i].position.y + tile_size / 2 }, tile_size / 4)) {
			switch (powerUp_tracker[i].type) {
			case 'U': //vida extra
				lives++;
				PlaySound(power_up_pick_up);
				break;
			case 'O'://monedes
				coins++;
				PlaySound(coin_sound);
				break;
			}
			//guardar power up al pool
			powerUp_pool.push_back(powerUp_tracker[i]);
			//borrar powerUp
			auto& k = powerUp_tracker.begin() + i;
			powerUp_tracker.erase(k);
		}
	}
}

void Player::shoot_bullets(Sound shoot_fx) {
	shoot_dir = { 0, 0 };
	//we get the direction of the bullet
	if (IsKeyDown(KEY_UP)) {
		shoot_dir = { shoot_dir.x, -1 };
		anim_dir = 4;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		shoot_dir = { shoot_dir.x, 1 };
		anim_dir = 2;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		shoot_dir = { 1, shoot_dir.y };
		anim_dir = 1;
	}
	else if (IsKeyDown(KEY_LEFT)) {
		shoot_dir = { -1, shoot_dir.y };
		anim_dir = 3;
	}

	//create bullets
	if ((shoot_dir.x != 0 || shoot_dir.y != 0) && fire_frame_counter % fire_rate == 0) {
		//first look for bullets in the pool
		PlaySound(shoot_fx);
		if (bullet_pool.empty()) {
			Bullet bullet({ rec.x, rec.y });
			bullet.set_damage(damage);
			bullet.set_velocity(shoot_dir);
			bullet_tracker.push_back(bullet);
		}
		else {
			bullet_tracker.push_back(bullet_pool.back());
			bullet_tracker.back().set_damage(damage);
			bullet_tracker.back().set_position({ rec.x + tile_size / 4 , rec.y + tile_size / 4 });
			bullet_tracker.back().set_velocity(shoot_dir);
			bullet_pool.pop_back();
		}
		fire_frame_counter++;
	}
	else if (fire_frame_counter % fire_rate != 0) {
		fire_frame_counter++;
	}
	//shoot_dir = { 0, 0 };
}

void Player::update_bullets() {
	//get the lenght of the bullet vector
	//update bullet's position
	for (int i = 0; i < bullet_tracker.size(); i++) {
		Bullet& b = bullet_tracker[i];
		if (b.get_velocity().x != 0 && b.get_velocity().y != 0) {
			b.set_position({ b.get_rec().x + b.get_velocity().x * b.get_speed() * 0.707f, b.get_rec().y + b.get_velocity().y * b.get_speed() * 0.707f});
		}
		else {
			b.set_position({ b.get_rec().x + b.get_velocity().x * b.get_speed(), b.get_rec().y + b.get_velocity().y * b.get_speed()});
		}
	}

	//destroy bullets
	for (int i = bullet_tracker.size() - 1; i >= 0; i--) {
		//iterate and check all bullets if they are ouside of the map (should I check if they colisioned?, might only have to check the first shot if we don't check colisions)
		if ((bullet_tracker[i].get_rec().x <= left_margin || bullet_tracker[i].get_rec().x >= area_size + (left_margin) || bullet_tracker[i].get_rec().y <= tile_size || bullet_tracker[i].get_rec().y >= area_size + tile_size)) { //&& bullet_tracker[i] != NULL
			//save the bullet in the pool
			bullet_pool.push_back(bullet_tracker[i]);
			//borrar bullet
			auto& j = bullet_tracker.begin() + i;
			bullet_tracker.erase(j);
		}

	}
}

void Player::bullet_attack(std::vector<Enemy>& enemy_tracker, std::vector<Enemy>& enemy_pool, std::vector<Bullet>& bullet_tracker, int& active_enemies) {
	//comprovar totes les bales per tots els enemics
	for (int i = enemy_tracker.size() - 1; i >= 0; i--) {
		for (int j = bullet_tracker.size() - 1; j >= 0; j--) {
			if (enemy_tracker.size() - 1 >= i && bullet_tracker.size() - 1 >= j && !enemy_tracker.empty() && !bullet_tracker.empty()) {
				if (CheckCollisionCircles({ bullet_tracker[j].get_rec().x + tile_size / 2, bullet_tracker[j].get_rec().y + tile_size / 2}, tile_size / 8, {enemy_tracker[i].get_rec().x + tile_size / 2, enemy_tracker[i].get_rec().y + tile_size / 2}, tile_size / 2)) { //MASSIVE ERROR
					//save the bullet in the pool
					bullet_pool.push_back(bullet_tracker[j]);
					//borrar bullet
					auto& k = bullet_tracker.begin() + j;
					bullet_tracker.erase(k);

					//reduce hit enemy hitpoints
					enemy_tracker[i].set_hp(enemy_tracker[i].get_hp() - damage);
					//kill enemy if hitpoints are 0 or lower
					if (enemy_tracker[i].get_hp() <= 0) {

						//save the enemy in the pool
						enemy_pool.push_back(enemy_tracker[i]);

						//mirar si es crea un power up
						spawnPowerUp(enemy_tracker[i].get_rec().x, enemy_tracker[i].get_rec().y);

						//crea un death animations object
						createDeathAnimation(enemy_tracker[i].get_rec());

						//borrar enemy
						auto& e = enemy_tracker.begin() + i;
						enemy_tracker.erase(e);
						active_enemies--;

					}
				}
			}

		}
	}
}

void Player::bullet_collision(std::vector<Obstacle>& obstacle_tracker) {
	for (int j = 0; j < obstacle_tracker.size(); j++) {
		for (int i = bullet_tracker.size() - 1; i >= 0; i--) {

			if (CheckCollisionCircles({ bullet_tracker[i].get_rec().x + tile_size / 2, bullet_tracker[i].get_rec().y + tile_size / 2}, tile_size / 8, {obstacle_tracker[j].get_rec().x + tile_size / 2, obstacle_tracker[j].get_rec().y + tile_size / 2}, tile_size / 2)) {
				//save the bullet in the pool
				bullet_pool.push_back(bullet_tracker[i]);
				//borrar bullet
				auto& k = bullet_tracker.begin() + i;
				bullet_tracker.erase(k);
			}
		}
	}
}