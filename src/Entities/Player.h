#pragma once
#include"IEntity.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"Image.h"
#include"Animation.h"
#include"GameContext.h"
#include"Collider.h"
#include"ColliderManager.h"
#include"Emitter.h"
#include"Danmaku.h"
#include"IInput.h"

class Player : public IEntity, public IRenderable, public IUpdateable
{
public:
    Player() = default;
    ~Player()
    {
        if (collider_)
        {
            ColliderManager::get_instance()->destroy_collider(collider_);
            collider_ = nullptr;
        }
    }

    Player(glm::vec2 pos, glm::vec2 vel, int hp, const Image& image, Collider* collider)
        : collider_(collider)
    {
        position_ = pos;
        velocity_ = vel;
        active_ = true;
        hp_ = 100;
        player_image_ = image;
        entity_type_ = EntityType::Player;
        collider_->set_on_collide([this](Collider* other)
        {
            alive_ = false;
            std::cout << "die" << std::endl;
        });
    }

    Player(glm::vec2 pos, glm::vec2 vel, int hp)
    {
        position_ = pos;
        velocity_ = vel;
        active_ = true;
        hp_ = 100;
        entity_type_ = EntityType::Player;
    }

    Player(PlayerContext pc)
    {
        active_ = pc.active;
        hp_ = pc.hp;
        position_ = pc.position;
        velocity_ = pc.velocity;
        alive_ = pc.is_alive;
    }

    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;

    void update(float delta_time) override;
    void render(SDLRender& renderer) const override;

    bool is_active() const override;

    glm::vec2 get_position() const override;
    void set_position(const glm::vec2& new_position) override;
    glm::vec2 get_velocity() const override;
    void set_velocity(const glm::vec2& new_velocity) override;
    void set_image(Image image);
    bool is_alive() const;
    void set_alive(bool alive);
    void set_hp(int hp);
    int get_hp() const;
    void set_animation(Animation animation);
    Animation& get_animation();
    void set_collider(Collider* collider);
    void handle_input();
    bool get_low_speed();
    void add_emitter(std::unique_ptr<Emitter> emitter);
    void set_bullet_pool(std::unique_ptr<DanmakuPool> player_bullet);
    Collider* get_collider();
    DanmakuPool* get_danmaku_pool();

private:
    bool alive_ = true;
    int hp_ = 100;
    Image player_image_;
    Animation player_animation_;
    bool is_low_speed_ = false;
    int shoot_cooldown_ = 0;

    Collider* collider_ = nullptr;
    std::unique_ptr<DanmakuPool> player_bullet_;
    std::vector<std::unique_ptr<Emitter>> emitters_;
};