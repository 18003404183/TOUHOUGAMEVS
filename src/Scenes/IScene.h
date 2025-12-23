#pragma once
#include"includes.h"
#include"IEntity.h"
#include"IRenderable.h"
#include"IUpdateable.h"


class IScene{
	
public:
	virtual ~IScene() = default;
	virtual void on_enter() = 0;
	virtual void on_exit() = 0;
	virtual void on_update(float deltatime) = 0;
	virtual void on_render() = 0;
	virtual void on_input() = 0;

protected:
	std::vector<std::unique_ptr<IEntity>> entities;
	std::vector<IRenderable*> renderables;
	std::vector<IUpdateable*> updateables;

	//每个场景必须拥有至少一个摄像机

	//每个场景必须维护一个自己的渲染器

	//加入脏标记 用于标记队列是否需要重新排序
	bool isRenderQueueDirty;
	bool isUpdateQueueDirty;


};