#pragma once
#include "IEntity.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "Image.h"


class Enemy :
    public IEntity,
    public IUpdateable,
	public IRenderable
{
public:
	Enemy() = default;
	virtual ~Enemy() override = default;


	void update(float deltaTime) override;
	void render(SDLRender& renderer) const override;

	virtual bool isActive() const;
	virtual void setActive(bool active);

	virtual glm::vec2 get_position() const;
	virtual void set_position(const glm::vec2& newPosition);
	virtual glm::vec2 get_velocity() const;
	virtual void set_velocity(const glm::vec2& newVelocity);
	virtual float setHp(int hp);
	virtual float getHp() const;
	virtual void takeDamage(float damage);

	virtual bool isAlive() const;
	virtual void setAlive(bool alive);

private:
	bool is_alive;
	float hp;
	Image image;
};

