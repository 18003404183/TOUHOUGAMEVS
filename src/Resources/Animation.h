#pragma once 

#include"clock.h"
#include"Atlas.h"
#include"IRenderable.h"
#include"IUpdateable.h"
#include"SDLRender.h"

class Animation :
public IUpdateable
{
private:
    
    //目前帧
    int current_index;
    //帧合集
    Atlas* atlas;
    //帧计时器
    Clock clock;
    //是否是循环动画
    bool is_loop;
    //一个动画帧持续多少游戏时
    float frame_speed;
    //动画缩放比例
    glm::vec2 scale;
    //渲染时在屏幕上的宽高
    glm::vec2 wh;

public:
    Animation() = default;

    Animation(Atlas* atlas,float frame_speed){
        this->current_index = 0;
        this->is_loop = true;
        this->atlas = atlas;
        //单位为秒
        this->frame_speed = frame_speed;

        clock.set_max_time(frame_speed);
        clock.set_one_shoot(!is_loop);
        clock.set_callback([this](){
            this->current_index = (this->current_index+1)%(this->atlas->get_max_index()); 
            std::cout<<"current_frame:"<<this->current_index<<std::endl;
        });

        this->scale = {1,1};
        //this->wh = {atlas->get_frame(current_index).w,atlas->get_frame(current_index).h};
        this->clock.start();

    }

    Animation(const Animation& animation){
        this->atlas = animation.atlas;
        this->clock = animation.clock;
        this->current_index = 0;
        this->frame_speed = animation.frame_speed;
        this->is_loop = animation.is_loop;
        this->scale = animation.scale;

        clock.set_max_time(frame_speed);
        clock.set_one_shoot(!is_loop);
        clock.set_callback([this](){
            this->current_index = (this->current_index+1)%(this->atlas->get_max_index());
            std::cout<<"current_frame:"<<this->current_index<<std::endl;
        });
        
    }

    Animation& operator=(const Animation& other){

        if(&other == this) return *this;

        this->atlas = other.atlas;
        this->clock = other.clock;
        this->current_index = 0;
        this->frame_speed = other.frame_speed;
        this->is_loop = other.is_loop;
        this->scale = other.scale;

        clock.set_max_time(frame_speed);
        clock.set_one_shoot(!is_loop);
        clock.set_callback([this](){
            this->current_index = (this->current_index+1)%(this->atlas->get_max_index());
            std::cout<<"current_frame:"<<this->current_index<<std::endl;
        });  

        return *this;
    }

    void update(float delta) override{

        this->clock.update(delta);

    }
    void render(SDLRender& renderer,glm::vec2 pos) const{

        if(!this->atlas){ 
            std::cerr << "no atlas" << std::endl; 
            return;
        };
        renderer.draw_texture(this->atlas->get_texture(),pos,this->scale,{this->atlas->get_frame(current_index).w,this->atlas->get_frame(current_index).h},this->atlas->get_frame(current_index),0,255);

    }


    ~Animation(){}

};
