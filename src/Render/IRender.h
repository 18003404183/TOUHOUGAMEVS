#pragma once

#include"Texture.h"
#include"Camera.h"
#include<string>

class Texture;

class IRender
{
public:
	
	IRender() = default;
	virtual ~IRender() = default;

	virtual void init(void* window = nullptr) = 0;

	virtual void clear() = 0;

	virtual Texture* load_texture(const std::string path) = 0;
	virtual void  draw_texture(Texture* image, int x, int y) = 0;
	virtual void draw_text(std::string, int x, int y) = 0;


protected:

};

