#include "EnemyBuilder.h"

void EnemyBuilder::create_enemy(EnemyType enemytype)
{
	if (enemytype == EnemyType::small) {
		std::cout << "Creating small enemy" << std::endl;
		this->active = true;
		this->hp = 10;
		this->is_alive = true;
		this->position = { 0,0 };
		this->velocity = { 10,0 };
	}
	else if (enemytype == EnemyType::common) {
		std::cout << "Creating common enemy" << std::endl;
		this->active = true;
		this->hp = 20;
		this->is_alive = true;
		this->position = { 10,0 };
		this->velocity = { 0,0 };
	}
	else if (enemytype == EnemyType::large) {
		std::cout << "Creating large enemy" << std::endl;
		this->active = true;
		this->hp = 30;
		this->is_alive = true;
		this->position = { 20,0 };
		this->velocity = { 0,0 };
	}
}

Enemy* EnemyBuilder::create()
{
	Enemy* enemy = new Enemy;
	enemy->setActive(this->active);
	enemy->setHp(this->hp);
	enemy->setAlive(this->is_alive);
	enemy->set_position(this->position);
	enemy->set_velocity(this->velocity);

	return enemy;

}
