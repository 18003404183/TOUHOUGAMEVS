#include "Player.h"

void Player::update(float delta_time)
{
	std::cout << "Player::update called with delta_time: " << delta_time << std::endl;
	this->position_ += this->velocity_ * delta_time;
	this->player_animation_.update(delta_time);
	if(this->player_bullet_){
		for(auto& emitter : this->emitters_){
			emitter.get()->update(delta_time, this->player_bullet_.get());
		}
		this->player_bullet_.get()->update(delta_time);
	}
	//this->player_image_.set_pos(position_);
}

void Player::render(SDLRender& renderer) const
{
	//this->player_image_.render(renderer, this->position_);
	//renderer.draw_texture(this->player_atlas_->get_texture(), this->position_, {1,1}, {200,200}, this->player_atlas_->get_frame(Context::instances()->game_frame % (this->player_atlas_->get_max_index())), 0, 255);
	this->player_animation_.render(renderer, this->position_);
	if (this->is_low_speed_ && this->alive_) {
        renderer.draw_circle_outline(this->position_, 6.0f, 255, 0, 0, 255);
        renderer.draw_circle_outline(this->position_, 4.0f, 255, 255, 255, 255);
    }
	if(this->player_bullet_){
		this->player_bullet_.get()->render(renderer);
	}
}

bool Player::is_active() const
{
	return this->active_;
}

glm::vec2 Player::get_position() const
{
	return this->position_;
}

void Player::set_position(const glm::vec2& new_position)
{
	this->position_ = new_position;
}

glm::vec2 Player::get_velocity() const
{
	return this->velocity_;
}

void Player::set_velocity(const glm::vec2& new_velocity)
{
	this->velocity_ = new_velocity;
}

void Player::set_image(Image image){
	this->player_image_ = image;
}

bool Player::is_alive() const
{
    return this->alive_;
}

void Player::set_alive(bool alive)
{
	this->alive_ = alive;
}

void Player::set_hp(int hp)
{
	this->hp_ = hp;
}

int Player::get_hp() const
{
	return this->hp_;
    return 0;
}

// void Player::set_atlas(Atlas *atlas)
// {
// 	this->player_atlas_ = atlas;
// }

void Player::set_animation(Animation animation)
{
	this->player_animation_ = animation;
}

Animation& Player::get_animation()
{
    return this->player_animation_;
}

void Player::set_collider(Collider* collider)
{
	this->collider_ = collider;
}

void Player::handle_input()
{
	if (!this->alive_ || !this->active_) return;

    // ===== 1. 处理移动与低速模式 (Shift) =====
    this->is_low_speed_ = IInput::get_key(KeyType::SHIFT)->get_state();
    
    // 注意：既然你在 update 里乘了 delta_time，这里的速度应该是"像素/秒"
    float current_speed = this->is_low_speed_ ? 250.0f : 550.0f; 

    glm::vec2 move_dir{0.0f, 0.0f};
    if (IInput::get_key(KeyType::RIGHT)->get_state()) move_dir.x += 1.0f;
    if (IInput::get_key(KeyType::LEFT)->get_state())  move_dir.x -= 1.0f;
    if (IInput::get_key(KeyType::DOWN)->get_state())  move_dir.y += 1.0f;
    if (IInput::get_key(KeyType::UP)->get_state())    move_dir.y -= 1.0f;

    // 归一化向量并直接赋值给 velocity
    if (glm::length(move_dir) > 0) {
        move_dir = glm::normalize(move_dir);
        this->velocity_ = move_dir * current_speed;
    } else {
        this->velocity_ = {0.0f, 0.0f}; // 松开按键时停下
    }

	bool is_shooting = IInput::get_key(KeyType::Z)->get_state();
    for (auto& emitter : this->emitters_) {
        emitter->set_active(is_shooting);
    }


}

bool Player::get_low_speed()
{
    return this->is_low_speed_;
}

void Player::add_emitter(std::unique_ptr<Emitter> emitter)
{
	emitter.get()->set_active(false);
	this->emitters_.push_back(std::move(emitter));
}

void Player::set_bullet_pool(std::unique_ptr<DanmakuPool> player_bullet)
{
	this->player_bullet_ = std::move(player_bullet);
}

Collider *Player::get_collider()
{
    return this->collider_;
}

DanmakuPool *Player::get_danmaku_pool()
{
	if(this->player_bullet_){
		return player_bullet_.get();
	}
    return nullptr;
}