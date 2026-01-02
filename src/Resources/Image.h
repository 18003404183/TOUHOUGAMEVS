#pragma once

#include"Texture.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"includes.h"

class SDLRender;

class Image :
	public IRenderable,
	public IUpdateable
{
public:
	Image() = default;
	Image(Texture* image):data(image){};
	~Image() = default;

	// 全属性构造函数
	Image(Texture* data,glm::vec2 pos,glm::vec2 scale,float rotation,unsigned char alpha):
	data(data),pos(pos),scale(scale),rotation(rotation),alpha(alpha)
	{
		this->wh = this->get_wh();
	}
	//get set 方法
	// data
	//inline Texture* get_data() { return data; }
	inline const Texture* get_data() const { return data; }
	inline void set_data(Texture* d) { data = d; this->wh = {data->width(),data->height()}; }

	// position
	inline const glm::vec2 get_pos() const { return pos; }
	inline void set_pos(const glm::vec2& p) { pos = p; }

	// scale
	inline glm::vec2 get_scale() const { return scale; }
	inline void set_scale(glm::vec2 s) { scale = s; }

	// rotation
	inline double get_rotation() const { return rotation; }
	inline void set_rotation(double r) { rotation = r; }

	// alpha
	inline unsigned char get_alpha() const { return alpha; }
	inline void set_alpha(unsigned char a) { alpha = a; }

	//得到图片的长度宽度
	inline glm::vec2 get_wh() const {
		if(!data) return {0,0};
		glm::vec2 a;
		a.x = this->data->width();
		a.y = this->data->height();
		return a;
	}

	//实现接口
	virtual void render(SDLRender& renderer) const override;
	virtual void render(SDLRender& renderer,glm::vec2 pos) const;

	virtual void update(float deltaTime);

private:
	Texture* data;//享元

	//长宽
	glm::vec2 wh;
	//位置float 是图像的中心点
	glm::vec2 pos;
	//缩放比例 float
	glm::vec2 scale;
	//旋转角度 float
	double rotation;
	//透明度  unsigned char
	unsigned char alpha = 255;
};

