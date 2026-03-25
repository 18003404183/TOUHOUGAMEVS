#include "Enemy.h"


void Enemy::update(float deltaTime)
{
	if(!this->isActive()||!this->is_alive){
		return;
	}
	this->position += this->velocity;
	this->enemy_animation.update(deltaTime);
	std::cout << "Enemy::update called with deltaTime: " << deltaTime << std::endl;
	if(this->enemy_danmaku_pool){
		for(auto& emitter : this->emitters){
			emitter.get()->update(deltaTime,this->enemy_danmaku_pool);
		}
	}
}

void Enemy::render(SDLRender& renderer) const
{
	if(!(this->active&&this->is_alive)){
		return;
	}
	std::cout << "Enemy::render called" << std::endl;
	//this->image.render(renderer,this->position);
	this->enemy_animation.render(renderer,this->position);
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


void Enemy::on_collision(){
	std::cout<<"enemy on collision"<<std::endl;
}

void Enemy::set_image(Image& image){
	this->image = image;
}

Image& Enemy::get_image(){
	return this->image;
}

bool Enemy::get_is_collision() const{
	return this->is_collision;
}

void Enemy::set_is_collision(bool collision){
	this->is_collision = collision;
}

void Enemy::add_emitter(std::unique_ptr<Emitter> emitter)
{
	this->emitters.push_back(std::move(emitter));
}

void Enemy::set_bullet_pool(DanmakuPool* pool)
{
	if(pool)
		this->enemy_danmaku_pool = pool;
}
