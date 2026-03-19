#pragma once
#include "Enemy.h"
#include "EnemyBuilder.h"
#include "EnemyType.h"
#include "IInput.h"
#include "IScene.h"
#include "Player.h"
// #include"EasyxRender.h"
#include "Clock.h"
#include "ColliderManager.h"
#include "Danmaku.h"
#include "EventManager.h"
#include <sol/sol.hpp>
class MainScene : public IScene {
public:
  MainScene();
  ~MainScene();

  virtual void on_enter();
  virtual void on_exit();
  virtual void on_update(float deltatime);
  virtual void on_render(SDLRender &renderer);
  virtual void on_input();

private:
  Clock clock;

  Player *player = nullptr;

  std::unique_ptr<DanmakuPool> enemy_bullets; 
  uint16_t test_prefab_id; // 存一下我们要发射的预制体 ID

  // ImGui 调试用的变量
  bool show_debug_hitbox = false;    // 碰撞盒开关
  int debug_emit_count = 12;         // 一圈发射多少颗子弹
  float debug_emit_speed = 150.0f;   // 子弹初速度
};
