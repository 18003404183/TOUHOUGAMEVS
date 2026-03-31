#pragma once
#include "includes.h"

enum class EntityType
{
    Player,
    Enemy,
    Danmaku
};

class IEntity
{
protected:
    glm::vec2 position_{0, 0};
    glm::vec2 velocity_{0, 0};
    bool active_ = true;
    EntityType entity_type_;

public:
    virtual bool is_active() const = 0;
    virtual EntityType get_entity_type() const { return entity_type_; }
    virtual ~IEntity() = default;
    virtual glm::vec2 get_position() const = 0;
    virtual glm::vec2* get_position_ptr() { return &position_; }
    virtual void set_position(const glm::vec2& new_position) = 0;
    virtual glm::vec2 get_velocity() const = 0;
    virtual void set_velocity(const glm::vec2& new_velocity) = 0;
};