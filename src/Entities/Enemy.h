#pragma once
#include "IEntity.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "Image.h"
#include "Collider.h"

class Enemy :
    public IEntity,
    public IUpdateable,
	public IRenderable
{
public:
	Enemy(){
		this->entity_type = EntityType::Enemy;
	};
	virtual ~Enemy() override = default;

	Collider* collider;

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

	void set_image(Image& image);
	Image& get_image();

	void set_shape(Shape* shape);
	const Shape* get_shape() const;

	void on_collision();

	bool get_is_collision() const;
	void set_is_collision(bool collision);

	

private:
	bool is_alive;
	float hp;
	Image image;
	Shape* shape;
	bool is_collision = false;
};

