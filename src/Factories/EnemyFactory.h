#pragma once
#include<unordered_map>
#include<glm.hpp>
#include<string>
#include"EntityConfig.h"
#include<iostream>


class Enemy;
class EnemyFactory{
private:
    std::unordered_map<std::string,EnemyConfig> prototypes_;

public:
    //跨场景考虑移动语义
    EnemyFactory() = default;
    EnemyFactory(const EnemyFactory& other) = delete;
    EnemyFactory& operator=(const EnemyFactory& other) = delete;
    //注册原型(提供通过json注册原型的接口 也可以通过手动输入参数来注册原型)
    EnemyFactory(EnemyFactory&& other) noexcept : prototypes_(std::move(other.prototypes_)){
        std::cout << "进行移动构造" << "\n";
    }

    EnemyFactory& operator=(EnemyFactory&& other) noexcept{
        if(this != &other){
            this->prototypes_ = std::move(other.prototypes_);
            other.prototypes_.clear();

            std::cout << "发生了移动赋值" << "\n";
        }

        return *this;
    }
    
    void load_json(const std::string& filepath);

    //根具原型生成一个对象 并且返回对象的指针 参数接收原型名称
    //如果有让敌人发射的子弹瞄准的需求 请自己手动设置敌人指向目标的glm::vec2 pos
    std::unique_ptr<Enemy> build_enemy(const std::string& enemy_name,float x,float y);

    ~EnemyFactory() = default;
};

