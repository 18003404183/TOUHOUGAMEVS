#pragma once
#include "Enemy.h"
#include "EnemyBuilder.h"
#include "EnemyType.h"
#include "IInput.h"
#include "IScene.h"
#include "Player.h"
#include "Clock.h"
#include "ColliderManager.h"
#include "Danmaku.h"
#include "EventManager.h"
#include <sol/sol.hpp>

class MainScene : public IScene
{
public:
    MainScene();
    ~MainScene();

    virtual void on_enter();
    virtual void on_exit();
    virtual void on_update(float delta_time);
    virtual void on_render(SDLRender& renderer);
    virtual void on_input();

private:
    Clock clock_;
    Player* player_ = nullptr;
    std::vector<std::unique_ptr<IEntity>> entities_;
    std::vector<IRenderable*> renderables_;
    std::vector<IUpdateable*> updateables_;

    std::unique_ptr<DanmakuPool> enemy_bullets_;
    uint16_t test_prefab_id_;

    // ImGui 调试用的变量
    bool show_debug_hitbox_ = false;
    int debug_emit_count_ = 12;
    float debug_emit_speed_ = 150.0f;
};