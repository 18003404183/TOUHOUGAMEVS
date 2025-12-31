#pragma once
#include"IEntity.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"Image.h"

class Player : public IEntity, public IRenderable, public IUpdateable
{
public:
	Player() = default;
	~Player() = default;

	Player(glm::vec2 pos,glm::vec2 v,int hp,Image image){
		this->position = pos;
		this->velocity = velocity;
		this->active = true;
		this->hp = 100;
		this->player_image = image;
	}

	void update(float deltaTime) override;
	void render(SDLRender& renderer) const override;

	virtual bool isActive() const;

	virtual glm::vec2 getPosition() const;
	virtual void setPosition(const glm::vec2& newPosition);
	virtual glm::vec2 getVelocity() const;
	virtual void setVelocity(const glm::vec2& newVelocity);
	virtual void setImage(Image image);

private:
	bool is_alive;
	int hp;
	Image player_image;
};

