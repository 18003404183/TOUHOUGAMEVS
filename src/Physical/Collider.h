#pragma once

#include"IEntity.h"
#include <functional>

enum class ShapeType{
    circle,
    rect
};

class Shape{
public:
    Shape(){};
    Shape(ShapeType shape_type):shape_type(shape_type){};
    virtual void set_shape(ShapeType shapetype){this->shape_type = shape_type;}
    virtual ShapeType get_shape(){return this->shape_type;}
    virtual Shape* clone() = 0;
    ~Shape() = default;
protected:
    ShapeType shape_type;
};

class Circle : public Shape{
public:

    Circle(){
        this->shape_type = ShapeType::circle;
    }
    Circle(float r){
        this->r = r;
        this->shape_type = ShapeType::circle;
    }
    float get_r(){
        return this->r;
    }
    void set_r(float r){
        this->r = r;
    }

    Shape* clone(){
        return new Circle(*this);
    }

protected:
    float r;
};

class Rectangle : public Shape{
public:
    Rectangle(){
        this->shape_type = ShapeType::rect;
    }

    Shape* clone(){
        return new Rectangle(*this);
    }
};

class Collider{
public:
    Collider() = delete;
    Collider(Shape* shape,int layer,std::function<void(Collider* other)> func,IEntity* entity);
    Collider(Shape* shape,int layer,IEntity* entity);
    Collider(const Collider& other);
    Collider& operator=(const Collider& other);
    void set_layer(int layer);
    void set_on_collid(std::function<void(Collider* other)> func);
    void run_on_collid(Collider* collider);
    void set_shape(Shape* shape);
    int get_layer();
    Shape* get_shape();
    void set_owner(IEntity* entity);
    IEntity* get_owner();
    ~Collider();
private:
    int layer;
    std::function<void(Collider* other)> on_collid;
    Shape* shape;

    //考虑加上持有者的信息 void* 但是这么做依赖于每个entity都有记录的自己的type
    IEntity* owner;
};
