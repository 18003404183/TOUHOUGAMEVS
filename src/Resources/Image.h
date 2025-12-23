#pragma once

#include"Texture.h"
#include"IRenderable.h"
#include"IUpdateable.h"
class Image :
	public IRenderable,
	public IUpdateable
{
public:
	Image() = default;
	Image(Texture* image) { 
		

	}

	// 全属性构造函数

	//get set 方法

	//实现接口
	virtual void render() const {};
	virtual void update(float deltaTime) {};

private:
	Texture* data;//享元

	//位置float

	//缩放比例 float

	//旋转角度 float

	//透明度  unsigned char


};

