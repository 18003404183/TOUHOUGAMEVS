#pragma once

#include"glm.hpp"
#include<string>

struct EmmiterConfig
{
    // 射击模式
    std::string aim_mode_;
    glm::vec2 offset_;
    // 配置弹幕
    uint16_t prefab_id_; // 种类
    float speed_;// 子弹初速度
    float base_angle_; // 初始角度 
    float angle_step_; // 角度变化量

    float interval_; // 发射间隔时间 也就是射速
    int count_; // 发射数量 -1表示无限发射

};

struct AnimationConfig
{
    std::string atlas_name_;
    float frame_speed_;
};


struct EnemyConfig
{

    EmmiterConfig emmiter_;
    AnimationConfig animation_;
    std::string image_name_;
    std::string enemy_animation_;
    float hp_;
    glm::vec2 pos_;
    glm::vec2 velocity_;
    // 这里考虑到目前的碰撞逻辑比较统一 于是不提供接口扩展碰撞逻辑 而是统一写死为同一条逻辑    
    std::string script_path_;
};
