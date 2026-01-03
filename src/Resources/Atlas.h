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

    ~Atlas(){
        delete this->texture;
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

