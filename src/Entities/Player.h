#pragma once
#include"IEntity.h"
#include"IRenderable.h"
#include"IUpdateable.h"

class Player : public IEntity, public IRenderable, public IUpdateable
{
public:
	Player() = default;
	~Player() = default;


	void update(float deltaTime) override;
	void render(SDLRender* renderer) const override;

	virtual bool isActive() const;

	virtual glm::vec2 getPosition() const;
	virtual void setPosition(const glm::vec2& newPosition);
	virtual glm::vec2 getVelocity() const;
	virtual void setVelocity(const glm::vec2& newVelocity);

private:
	bool is_alive;
	bool hp;

};

