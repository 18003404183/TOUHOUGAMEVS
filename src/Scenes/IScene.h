#pragma once
#include"includes.h"
#include"IEntity.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"SDLRender.h"

class IScene{
	
public:
	virtual ~IScene() = default;
	virtual void on_enter() = 0;
	virtual void on_exit() = 0;
	virtual void on_update(float deltatime) = 0;
	virtual void on_render(SDLRender& renderer) = 0;
	virtual void on_input() = 0;

protected:
	std::vector<std::unique_ptr<IEntity>> entities;
	std::vector<IRenderable*> renderables;
	std::vector<IUpdateable*> updateables;

	//ÿ����������ӵ������һ�������?

	//ÿ����������ά��һ���Լ�����Ⱦ��

	//�������� ���ڱ�Ƕ����Ƿ���Ҫ��������?
	bool isRenderQueueDirty;
	bool isUpdateQueueDirty;


};