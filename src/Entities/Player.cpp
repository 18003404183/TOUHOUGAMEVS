#include "Player.h"

void Player::update(float deltaTime)
{
	std::cout << "Player::update called with deltaTime: " << deltaTime << std::endl;
	this->position+=this->velocity*deltaTime;
	this->player_animation.update(deltaTime);
	if(this->player_bullet){
		for(auto& emitter : this->emitters){
			emitter.get()->update(deltaTime,this->player_bullet.get());
		}
		this->player_bullet.get()->update(deltaTime);
	}
	//this->player_image.set_pos(position);
}

void Player::render(SDLRender& renderer) const
{
	//this->player_image.render(renderer,this->position);
	//renderer.draw_texture(this->player_atlas->get_texture(),this->position,{1,1},{200,200},this->player_atlas->get_frame(Context::Instances()->game_frame%(this->player_atlas->get_max_index())),0,255);
	this->player_animation.render(renderer,this->position);
	if (this->is_low_speed && this->alive) {
        renderer.draw_circle_outline(this->position, 6.0f, 255, 0, 0, 255);
        renderer.draw_circle_outline(this->position, 4.0f, 255, 255, 255, 255);
    }
	if(this->player_bullet){
		this->player_bullet.get()->render(renderer);
	}
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

void Player::handle_input()
{
	if (!this->alive || !this->active) return;

    // ===== 1. 处理移动与低速模式 (Shift) =====
    this->is_low_speed = IInput::get_key(KeyType::SHIFT)->get_state();
    
    // 注意：既然你在 update 里乘了 deltaTime，这里的速度应该是“像素/秒”
    float current_speed = this->is_low_speed ? 250.0f : 550.0f; 

    glm::vec2 move_dir{0.0f, 0.0f};
    if (IInput::get_key(KeyType::RIGHT)->get_state()) move_dir.x += 1.0f;
    if (IInput::get_key(KeyType::LEFT)->get_state())  move_dir.x -= 1.0f;
    if (IInput::get_key(KeyType::DOWN)->get_state())  move_dir.y += 1.0f;
    if (IInput::get_key(KeyType::UP)->get_state())    move_dir.y -= 1.0f;

    // 归一化向量并直接赋值给 velocity
    if (glm::length(move_dir) > 0) {
        move_dir = glm::normalize(move_dir);
        this->velocity = move_dir * current_speed;
    } else {
        this->velocity = {0.0f, 0.0f}; // 松开按键时停下
    }

	bool is_shooting = IInput::get_key(KeyType::Z)->get_state();
    for (auto& emitter : this->emitters) {
        emitter->set_active(is_shooting);
    }


}

bool Player::get_low_speed()
{
    return this->is_low_speed;
}

void Player::add_emitter(std::unique_ptr<Emitter> emitter)
{
	emitter.get()->set_active(false);
	this->emitters.push_back(std::move(emitter));
}

void Player::set_bullet_pool(std::unique_ptr<DanmakuPool> player_bu)
{
	this->player_bullet = std::move(player_bu);
}

Collider *Player::get_collider()
{
    return this->collider;
}

DanmakuPool *Player::get_danmaku_pool()
{
	if(this->player_bullet){
		return player_bullet.get();
	}
    return nullptr;
}
