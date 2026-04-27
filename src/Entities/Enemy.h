#pragma once
#include "IEntity.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "Image.h"
#include "Collider.h"
#include "Animation.h"
#include "ColliderManager.h"
#include "Emitter.h"
#include "Danmaku.h"

class Enemy : public IEntity, public IUpdateable, public IRenderable
{
public:
    Enemy()
    {
        entity_type_ = EntityType::Enemy;
    }

    virtual ~Enemy() override
    {
        if (collider_)
        {
            ColliderManager::get_instance()->destroy_collider(collider_);
            collider_ = nullptr;
        }
    }

    Collider* collider_ = nullptr;

    void update(float delta_time) override;
    void render(SDLRender& renderer) const override;

    virtual bool is_active() const override;
    virtual void set_active(bool active);

    virtual glm::vec2 get_position() const override;
    virtual void set_position(const glm::vec2& new_position) override;
    virtual glm::vec2 get_velocity() const override;
    virtual void set_velocity(const glm::vec2& new_velocity) override;
    virtual float set_hp(int hp);
    virtual float get_hp() const;
    virtual void take_damage(float damage);

    virtual bool is_alive() const;
    virtual void set_alive(bool alive);

    void set_image(const Image& image);
    Image& get_image();

    void on_collision();

    bool get_is_collision() const;
    void set_is_collision(bool collision);

    void add_emitter(std::unique_ptr<Emitter> emitter);
    void set_bullet_pool(DanmakuPool* pool);

    void set_animation(const Animation& animation)
    {   
        enemy_animation_ = animation;
    }

private:
    bool is_alive_ = true;
    float hp_ = 100.0f;
    Image image_;
    bool is_collision_ = false;
    Animation enemy_animation_;

    std::vector<std::unique_ptr<Emitter>> emitters_;
    DanmakuPool* enemy_danmaku_pool_ = nullptr;
};