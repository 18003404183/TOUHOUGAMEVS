#pragma once

#include"glm.hpp"
#include"unordered_map"
#include<string>
#include"json.hpp"

using json = nlohmann::json;

struct EmitterConfig
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
    std::unordered_map<std::string,EmitterConfig> emitters_;
    AnimationConfig animation_;
    std::string image_name_;
    float hp_;
    glm::vec2 velocity_;
    // 这里考虑到目前的碰撞逻辑比较统一 于是不提供接口扩展碰撞逻辑 而是统一写死为同一条逻辑    
    std::string script_path_;
};

inline glm::vec2 parse_vec2(const json& js,float default_x = 0.0f,float default_y = 0.0f){
    if(js.is_null()) return {default_x,default_y};
    return {
        js.value("x",default_x),
        js.value("y",default_y)
    };
}

inline AnimationConfig parse_animation_config(const json& js){
    AnimationConfig config;
    if(js.is_null()) return config;
    config.atlas_name_ = js.value("atlas_name","");
    config.frame_speed_ = js.value("frame_speed",0.1f);

    return config;
}


inline EmitterConfig parse_Emitter_config(const json& js){
    EmitterConfig config;
    if(js.is_null()) return config;
    config.aim_mode_      = js.value("aim_mode","Fixed");
    if(js.contains("offset")){
        config.offset_    = parse_vec2(js["offset"]);
    }
    else{
        config.offset_    = {0.0,0.0};
    }
    config.prefab_id_     = js.value("prefab_id",0);
    config.speed_         = js.value("speed",0.0f);
    config.base_angle_    = js.value("base_angle",0.0f);
    config.angle_step_    = js.value("angle_step",0.0f);
    config.interval_      = js.value("interval",0.0f);
    config.count_         = js.value("count",-1);

    return config;
}   

