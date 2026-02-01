#include "EnemyBuilder.h"
#include"ResourcesManager.h"

void EnemyBuilder::create_enemy(EnemyType enemytype)
{
	if (enemytype == EnemyType::small) {
		std::cout << "Creating small enemy" << std::endl;
		this->active = true;
		this->hp = 10;
		this->is_alive = true;
		this->position = { 0,0 };
		this->velocity = { 10,0 };
		this->shape = new CircleShape(500);
		Texture* t = ResourcesManager::getInstance()->get_texture("resources\\2.png");	
		Image image(t,this->position,{1,1},0,255);
		this->image = image;
	}
	else if (enemytype == EnemyType::common) {
		std::cout << "Creating common enemy" << std::endl;
		this->active = true;
		this->hp = 20;
		this->is_alive = true;
		this->position = { 10,0 };
		this->velocity = { 0,0 };
		this->shape = new CircleShape(1000);
		Texture* t = ResourcesManager::getInstance()->get_texture("resources\\3.png");	
		Image image(t,this->position,{1,1},0,255);
		this->image = image;
	}
	else if (enemytype == EnemyType::large) {
		std::cout << "Creating large enemy" << std::endl;
		this->active = true;
		this->hp = 30;
		this->is_alive = true;
		this->position = { 20,0 };
		this->velocity = { 0,0 };
		this->shape = new CircleShape(1500);
		Texture* t = ResourcesManager::getInstance()->get_texture("resources\\4.png");	
		Image image(t,this->position,{1,1},0,255);
		this->image = image;
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
	enemy->set_shape(this->shape);
	enemy->set_image(this->image);

	return enemy;

}
