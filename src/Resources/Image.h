#pragma once

#include"Texture.h"
#include"IRenderable.h"
#include"IUpdateable.h"
//#include"includes.h"

class Image :
	public IRenderable,
	public IUpdateable
{
public:
	Image() = default;
	Image(Texture* image):data(image){};
	~Image() = default;

	// 全属性构造函数

	//get set 方法

	//实现接口
	virtual void render(SDLRender* renderer) const override{};
	virtual void update(float deltaTime){};

private:
	Texture* data;//享元

	//位置float
	//glm::vec2 pos;
	//缩放比例 float
	
	//旋转角度 float

	//透明度  unsigned char


};

