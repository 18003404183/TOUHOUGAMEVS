#pragma once
#include "EnemyType.h"
#include "Enemy.h"
#include "EventManager.h"

class EnemyBuilder
{
public:
    void create_enemy(EnemyType enemy_type);
    Enemy* create();

private:
    float hp_ = 100.0f;
    bool is_alive_ = true;
    glm::vec2 position_{0, 0};
    glm::vec2 velocity_{0, 0};
    bool active_ = true;

    Collider* collider_ = nullptr;
    Shape* shape_ = nullptr;
    Image image_;
};