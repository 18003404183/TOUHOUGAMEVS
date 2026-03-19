#pragma once
#include "IEntity.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "Image.h"
#include "Collider.h"
#include "Animation.h"
#include "ColliderManager.h"
class Enemy :
    public IEntity,
    public IUpdateable,
	public IRenderable
{
public:
	Enemy(){
		this->entity_type = EntityType::Enemy;
	};
	virtual ~Enemy() override{
		if(this->collider){
			ColliderManager::get_instance()->destory_collider(this->collider);
			this->collider = nullptr;
		}
	};

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

	void on_collision();

	bool get_is_collision() const;
	void set_is_collision(bool collision);

	void set_animation(const Animation& animation){
		this->enemy_animation = animation;
	}

private:
	bool is_alive;
	float hp;
	Image image;
	bool is_collision = false;
	Animation enemy_animation;
};

