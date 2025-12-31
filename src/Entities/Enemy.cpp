#include "Enemy.h"


void Enemy::update(float deltaTime)
{
	std::cout << "Enemy::update called with deltaTime: " << deltaTime << std::endl;
}

void Enemy::render(SDLRender& renderer) const
{
	std::cout << "Enemy::render called" << std::endl;
}

bool Enemy::isActive() const
{
	return this->active;
}

void Enemy::setActive(bool active)
{
	this->active = active;
}

glm::vec2 Enemy::getPosition() const
{
	return this->position;
}

void Enemy::setPosition(const glm::vec2& newPosition)
{
	this->position = newPosition;
}

glm::vec2 Enemy::getVelocity() const
{
	return this->velocity;
}

void Enemy::setVelocity(const glm::vec2& newVelocity)
{
	this->velocity = newVelocity;
}

float Enemy::setHp(int hp)
{
	return this->hp = hp;
}

float Enemy::getHp() const
{
	return this->hp;
}

void Enemy::takeDamage(float damage)
{
	this->hp -= damage;
}

bool Enemy::isAlive() const
{
	return this->is_alive;
}

void Enemy::setAlive(bool alive)
{
	this->is_alive = alive;
}
