#include "EnemyBuilder.h"
#include"ResourcesManager.h"
#include"ColliderManager.h"

void EnemyBuilder::create_enemy(EnemyType enemy_type)
{
	if (enemy_type == EnemyType::small) {
		std::cout << "Creating small enemy" << std::endl;
		this->active_ = true;
		this->hp_ = 10;
		this->is_alive_ = true;
		this->position_ = { 200,200 };
		this->velocity_ = { 10,0 };
		// this->shape_ = new CircleShape(500);
		Texture* t = ResourcesManager::get_instance()->get_texture("resources\\2.png");	
		Image image(t, this->position_, {1,1}, 0, 255);
		this->image_ = image;
		this->collider_ = ColliderManager::get_instance()->create_collider(new Circle(30), 1, nullptr);
	}
	else if (enemy_type == EnemyType::common) {
		std::cout << "Creating common enemy" << std::endl;
		this->active_ = true;
		this->hp_ = 20; 
		this->is_alive_ = true;
		this->position_ = { 300,0 };
		this->velocity_ = { 0,0 };
		// this->shape_ = new CircleShape(1000);
		Texture* t = ResourcesManager::get_instance()->get_texture("resources\\3.png");	
		Image image(t, this->position_, {1,1}, 0, 255);
		this->image_ = image;
		this->collider_ = ColliderManager::get_instance()->create_collider(new Circle(30), 1, nullptr);
	}
	else if (enemy_type == EnemyType::large) {
		std::cout << "Creating large enemy" << std::endl;
		this->active_ = true;
		this->hp_ = 30;
		this->is_alive_ = true;
		this->position_ = { 100,0 };
		this->velocity_ = { 0,0 };
		// this->shape_ = new CircleShape(1500);
		Texture* t = ResourcesManager::get_instance()->get_texture("resources\\4.png");	
		Image image(t, this->position_, {1,1}, 0, 255);
		this->image_ = image;
		this->collider_ = ColliderManager::get_instance()->create_collider(new Circle(30), 1, nullptr);

	}
}

Enemy* EnemyBuilder::create()
{
	Enemy* enemy = new Enemy;
	enemy->set_active(this->active_);
	enemy->set_hp(this->hp_);
	enemy->set_alive(this->is_alive_);
	enemy->set_position(this->position_);
	enemy->set_velocity(this->velocity_);
	enemy->set_image(this->image_);
	Animation animation(ResourcesManager::get_instance()->get_atlas("resources\\7.png"), 0.1);
	enemy->set_animation(animation);
	enemy->collider_ = collider_;
	enemy->collider_->set_owner(enemy);
	enemy->collider_->set_on_collide([e = enemy](Collider* other){
		e->take_damage(10);
		if(e->get_hp() <= 0 && e->is_alive()){
			e->set_alive(false);
			
			Event ev;
			ev.type = EventType::EnemyDsetroyed;
			ev.data = e;
			EventManager::get_instance()->publish(ev);
		}		
	}
	);
	return enemy;

}