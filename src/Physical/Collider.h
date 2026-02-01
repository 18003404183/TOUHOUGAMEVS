#pragma once

//碰撞实现思路
//在这里写一些struct 来定义形状的各种数学数据
//写一个算法库 里面的函数专门负责判断是否碰撞
//写一个碰撞管理器 用于检测 并执行 实体间的碰撞
//对于子弹等大量出现的实体 写一个形状枚举和碰撞后执行逻辑的枚举
// 在ColliderManager里使用 vector<shape*> vector<function*> 来存储 需要的时候访问

enum class ShapeType {
    Circle,
    Box
};


struct Shape {
    ShapeType type;
    Shape(ShapeType t) : type(t) {}
    virtual ~Shape() = default;
};

// 圆形数据
struct CircleShape : public Shape {
    float radius;
    CircleShape(float r) : Shape(ShapeType::Circle), radius(r) {}
};

// 矩形数据
struct BoxShape : public Shape {
    float width, height;
    BoxShape(float w, float h) : Shape(ShapeType::Box), width(w), height(h) {}
};