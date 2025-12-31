#include "Player.h"

void Player::update(float deltaTime)
{
	std::cout << "Player::update called with deltaTime: " << deltaTime << std::endl;
	this->position+=this->velocity*deltaTime;
	this->player_image.setPos(position);
}

void Player::render(SDLRender& renderer) const
{
	std::cout << "Player::render called" << std::endl;
	this->player_image.render(renderer);
}

bool Player::isActive() const
{
	return this->active;
}

glm::vec2 Player::getPosition() const
{
	return this->position;
}

void Player::setPosition(const glm::vec2& newPosition)
{
	this->position = newPosition;
}

glm::vec2 Player::getVelocity() const
{
	return this->velocity;
}

void Player::setVelocity(const glm::vec2& newVelocity)
{
	this->velocity = newVelocity;
}

void Player::setImage(Image image){
	this->player_image = image;
}
