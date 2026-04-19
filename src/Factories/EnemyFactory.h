#pragma once
#include<unordered_map>
#include<glm.hpp>
#include<string>
#include"EntityConfig.h"

class EnemyFactory{
private:
    std::unordered_map<std::string,EnemyConfig> prototypes_;

public:

    //注册原型(提供通过json注册原型的接口 也可以通过手动输入参数来注册原型)

    //根具原型生成一个对象 并且返回对象的指针 参数接收原型名称


};