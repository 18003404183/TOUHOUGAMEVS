#pragma once
#include"SDLRender.h"

class SDLRender;

class IRenderable {
public:

	virtual ~IRenderable() = default;
	virtual void render(SDLRender* renderer) const = 0;

protected:
	//���ȼ� ���ȼ��ߵĺ����(��ʾ�����ϲ�)
	float render_priority;
};