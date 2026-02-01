#pragma once
#include"IEntity.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"Image.h"
#include"Animation.h"
#include"GameContext.h"
#include"Collider.h"

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

	Player(PlayerContext pc){
		this->active = pc.active;
		this->hp = pc.hp;
		this->position = pc.position;
		//this->player_image = pc.player_image;
		this->velocity = pc.velocity;
		this->alive = pc.is_alive;
	}

	Player(const Player& other){

	}

	Player& operator=(const Player& other){

	}


	void update(float deltaTime) override;
	void render(SDLRender& renderer) const override;

	virtual bool isActive() const;

	virtual glm::vec2 get_position() const;
	virtual void set_position(const glm::vec2& newPosition);
	virtual glm::vec2 get_velocity() const;
	virtual void set_velocity(const glm::vec2& newVelocity);
	virtual void setImage(Image image);
	virtual bool is_alive() const;
	virtual void set_alive(bool alive);
	virtual void set_hp(int hp);
	virtual int get_hp() const;
	//virtual void set_atlas(Atlas* atlas);
	virtual void set_animation(Animation animation);
	virtual Animation& get_animation();
	virtual const Shape* get_shape() const;
	virtual void set_shape(Shape* shape);
	//碰撞后执行的逻辑
	void on_collision();

private:

	bool alive;
	int hp;
	Image player_image;
	Animation player_animation;
	Shape* shape;

};

