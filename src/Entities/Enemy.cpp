#include "Enemy.h"


void Enemy::update(float deltaTime)
{
	this->position += this->velocity;
	std::cout << "Enemy::update called with deltaTime: " << deltaTime << std::endl;
}

void Enemy::render(SDLRender& renderer) const
{
	std::cout << "Enemy::render called" << std::endl;
	this->image.render(renderer,this->position);
}

bool Enemy::isActive() const
{
	return this->active;
}

void Enemy::setActive(bool active)
{
	this->active = active;
}

glm::vec2 Enemy::get_position() const
{
	return this->position;
}

void Enemy::set_position(const glm::vec2& newPosition)
{
	this->position = newPosition;
}

glm::vec2 Enemy::get_velocity() const
{
	return this->velocity;
}

void Enemy::set_velocity(const glm::vec2& newVelocity)
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

void Enemy::set_shape(Shape* shape){
	this->shape = shape;
}

const Shape* Enemy::get_shape() const{
	return this->shape;
}

void Enemy::on_collision(){
	std::cout<<"敌人发生碰撞"<<std::endl;
}

void Enemy::set_image(Image& image){
	this->image = image;
}

Image& Enemy::get_image(){
	return this->image;
}