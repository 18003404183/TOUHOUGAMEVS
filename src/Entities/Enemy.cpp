#include "Enemy.h"


void Enemy::update(float delta_time)
{
	if(!this->is_active() || !this->is_alive_){
		return;
	}
	this->position_ += this->velocity_;
	this->enemy_animation_.update(delta_time);
	std::cout << "Enemy::update called with delta_time: " << delta_time << std::endl;
	if(this->enemy_danmaku_pool_){
		for(auto& emitter : this->emitters_){
			emitter.get()->update(delta_time, this->enemy_danmaku_pool_);
		}
	}
}

void Enemy::render(SDLRender& renderer) const
{
	if(!(this->active_ && this->is_alive_)){
		return;
	}
	std::cout << "Enemy::render called" << std::endl;
	//this->image_.render(renderer, this->position_);
	this->enemy_animation_.render(renderer, this->position_);
}

bool Enemy::is_active() const
{
	return this->active_;
}

void Enemy::set_active(bool active)
{
	this->active_ = active;
}

glm::vec2 Enemy::get_position() const
{
	return this->position_;
}


void Enemy::set_position(const glm::vec2& new_position)
{
	this->position_ = new_position;
}

glm::vec2 Enemy::get_velocity() const
{
	return this->velocity_;
}

void Enemy::set_velocity(const glm::vec2& new_velocity)
{
	this->velocity_ = new_velocity;
}

float Enemy::set_hp(int hp)
{
	return this->hp_ = hp;
}

float Enemy::get_hp() const
{
	return this->hp_;
}

void Enemy::take_damage(float damage)
{
	this->hp_ -= damage;
}

bool Enemy::is_alive() const
{
	return this->is_alive_;
}

void Enemy::set_alive(bool alive)
{
	this->is_alive_ = alive;
}


void Enemy::on_collision(){
	std::cout << "enemy on collision" << std::endl;
}

void Enemy::set_image(const Image& image){
	this->image_ = image;
}

Image& Enemy::get_image(){
	return this->image_;
}

bool Enemy::get_is_collision() const{
	return this->is_collision_;
}

void Enemy::set_is_collision(bool collision){
	this->is_collision_ = collision;
}

void Enemy::add_emitter(std::unique_ptr<Emitter> emitter)
{
	this->emitters_.push_back(std::move(emitter));
}

void Enemy::set_bullet_pool(DanmakuPool* pool)
{
	if(pool)
		this->enemy_danmaku_pool_ = pool;
}