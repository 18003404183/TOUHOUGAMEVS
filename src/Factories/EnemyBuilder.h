#pragma once
#include"EnemyType.h"
#include"Enemy.h"
#include "EventManager.h"

class EnemyBuilder
{
public:
	void create_enemy(EnemyType enemytype);
	Enemy* create();

private:
	float hp;
	bool is_alive;
	glm::vec2 position;
	glm::vec2 velocity;
	bool active;

	Collider* collider;

	Shape* shape;
	Image image;
	
};

