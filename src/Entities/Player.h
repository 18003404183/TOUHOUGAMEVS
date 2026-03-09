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

	Player(glm::vec2 pos,glm::vec2 v,int hp,const Image& image,Collider* collider):collider(collider){
		this->position = pos;
		this->velocity = v;
		this->active = true;
		this->hp = 100;
		this->player_image = image;
		this->entity_type = EntityType::Player;
		this->collider->set_on_collid([this](Collider* other){
			this->alive = false;
			std::cout << "die" << std::endl; 
		});
	}

	Player(glm::vec2 pos,glm::vec2 v,int hp){
		this->position = pos;
		this->velocity = v;
		this->active = true;
		this->hp = 100;
		this->entity_type = EntityType::Player;
	}

	Player(PlayerContext pc){
		this->active = pc.active;
		this->hp = pc.hp;
		this->position = pc.position;
		//this->player_image = pc.player_image;
		this->velocity = pc.velocity;
		this->alive = pc.is_alive;
	}

	Player(const Player& other) = delete;
	Player& operator=(const Player& other) = delete;


	void update(float deltaTime) override;
	void render(SDLRender& renderer) const override;

	bool isActive() const;

	glm::vec2 get_position() const;
	void set_position(const glm::vec2& newPosition);
	glm::vec2 get_velocity() const;
	void set_velocity(const glm::vec2& newVelocity);
	void setImage(Image image);
	bool is_alive() const;
	void set_alive(bool alive);
	void set_hp(int hp);
	int get_hp() const;
	//virtual void set_atlas(Atlas* atlas);
	void set_animation(Animation animation);
	Animation& get_animation();
	//virtual const Shape* get_shape() const;
	//virtual void set_shape(Shape* shape);
	void set_collider(Collider* collider);
	Collider* get_collider();
private:
	bool alive;
	int hp;
	Image player_image;
	Animation player_animation;
	//Shape* shape;
	Collider* collider;
};

