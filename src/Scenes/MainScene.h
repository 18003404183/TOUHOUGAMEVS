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
};
