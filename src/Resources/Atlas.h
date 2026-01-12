#pragma once

#include"Texture.h"
#include"includes.h"

//分割texture 成为atlas

class Atlas
{
public:
    Atlas() = default;
    Atlas(Texture* texture){
        this->texture = texture;
        this->wh = {this->texture->width(),this->texture->height()};
    }

    Atlas(Texture* texture,glm::vec2 cell){
        this->texture = texture;
        glm::vec2 step = {this->get_texture()->width()/cell.x,this->get_texture()->height()/cell.y};
		for(float i = 1;i<2;i++){
			for(float j = 0;j<cell.x;j++){
				this->add_frame({(int)(j*step.x),(int)(i*step.y),(int)(step.x),(int)(step.y)});
			}
		}
    }

    ~Atlas(){
        //delete this->texture;
    }

    //这里需要按照顺序加入图片
    void add_frame(SDL_Rect cell){
        this->frames.push_back(cell);
    }

    SDL_Rect get_frame(int index){
        return this->frames[index];
    }

    void clear_frames(){
        this->frames.clear();
    }

    Texture* get_texture(){
        return this->texture;
    }

    void set_texture(Texture* texture){
        this->texture = texture;
        this->wh = {texture->width(),texture->height()};
    }

    int get_max_index(){
        return this->frames.size();
    }

private:
    std::vector<SDL_Rect> frames;
    glm::vec2 wh;
    Texture* texture;
};

