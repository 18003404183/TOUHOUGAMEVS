#pragma once

#include"IRender.h"
#include<SDL.h>
#include"includes.h"

class SDLRender
{
public:
	SDLRender() = default;

	virtual void init(SDL_Window* window = nullptr);

	virtual void clear();

	//virtual Texture* load_texture(const std::string path);
	virtual void  draw_texture(Texture* image, int x, int y);
	virtual void draw_texture(
        Texture* texture, 
        const glm::vec2& pos, 
        const glm::vec2& scale,
        const glm::vec2& wh,
        double rotation, 
        unsigned char alpha);
	virtual void draw_text(std::string text, int x, int y);

	SDL_Renderer* get_renderer(){
		return this->renderer;
	}

	void present();

private:
	SDL_Renderer* renderer = nullptr;
};