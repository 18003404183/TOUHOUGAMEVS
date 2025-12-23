#pragma once
#include"IScene.h"
#include"Enemy.h"
#include"Player.h"
#include"EnemyBuilder.h"
#include"EnemyType.h"
#include"IInput.h"

class EndScene :
    public IScene
{
public:
	EndScene();
	~EndScene();

	virtual void on_enter();
	virtual void on_exit();
	virtual void on_update(float deltatime);
	virtual void on_render();
	virtual void on_input();
};

