#include "Player.h"

void Player::update(float deltaTime)
{
	std::cout << "Player::update called with deltaTime: " << deltaTime << std::endl;
	this->position+=this->velocity*deltaTime;
	this->player_animation.update(deltaTime);
	//this->player_image.set_pos(position);
}

void Player::render(SDLRender& renderer) const
{
	std::cout << "Player::render called" << std::endl;
	//this->player_image.render(renderer,this->position);
	//renderer.draw_texture(this->player_atlas->get_texture(),this->position,{1,1},{200,200},this->player_atlas->get_frame(Context::Instances()->game_frame%(this->player_atlas->get_max_index())),0,255);
	this->player_animation.render(renderer,this->position);
}

bool Player::isActive() const
{
	return this->active;
}

glm::vec2 Player::get_position() const
{
	return this->position;
}

void Player::set_position(const glm::vec2& newPosition)
{
	this->position = newPosition;
}

glm::vec2 Player::get_velocity() const
{
	return this->velocity;
}

void Player::set_velocity(const glm::vec2& newVelocity)
{
	this->velocity = newVelocity;
}

void Player::setImage(Image image){
	this->player_image = image;
}

bool Player::is_alive() const
{
    return this->alive;
}

void Player::set_alive(bool alive)
{
	this->alive = alive;
}

void Player::set_hp(int hp)
{
	this->hp = hp;
}

int Player::get_hp() const
{
	return this->hp;
    return 0;
}

// void Player::set_atlas(Atlas *atlas)
// {
// 	this->player_atlas = atlas;
// }

void Player::set_animation(Animation animation)
{
	this->player_animation = animation;
}

Animation& Player::get_animation()
{
    return this->player_animation;
}

void Player::set_collider(Collider* collider)
{
	this->collider = collider;
}

Collider *Player::get_collider()
{
    return this->collider;
}
