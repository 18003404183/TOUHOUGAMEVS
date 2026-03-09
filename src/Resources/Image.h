#pragma once

#include"Texture.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"includes.h"

class SDLRender;

class Image :
	public IRenderable
{
public:
	Image() = default; 
	Image(Texture* image):data(image){};
	~Image() = default;

	// ȫ���Թ��캯��
	Image(Texture* data,glm::vec2 pos,glm::vec2 scale,float rotation,unsigned char alpha):
	data(data),pos(pos),scale(scale),rotation(rotation),alpha(alpha)
	{
		this->wh = this->get_wh();
	}
	//get set ����
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

	//�õ�ͼƬ�ĳ��ȿ���
	inline glm::vec2 get_wh() const {
		if(!data) return {0,0};
		glm::vec2 a;
		a.x = this->data->width();
		a.y = this->data->height();
		return a;
	}

	//ʵ�ֽӿ�
	virtual void render(SDLRender& renderer) const override;
	virtual void render(SDLRender& renderer,glm::vec2 pos) const;

	//virtual void update(float deltaTime);

private:
	Texture* data;//��Ԫ

	//����
	glm::vec2 wh;
	//λ��float ��ͼ������ĵ�
	glm::vec2 pos;
	//���ű��� float
	glm::vec2 scale;
	//��ת�Ƕ� float
	double rotation;
	//͸����  unsigned char
	unsigned char alpha = 255;
};

