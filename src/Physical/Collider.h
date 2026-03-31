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
    Shape(ShapeType shape_type):shape_type_(shape_type){};
    virtual void set_shape(ShapeType shape_type){this->shape_type_ = shape_type;}
    virtual ShapeType get_shape(){return this->shape_type_;}
    virtual Shape* clone() = 0;
    ~Shape() = default;
protected:
    ShapeType shape_type_;
};

class Circle : public Shape{
public:

    Circle(){
        this->shape_type_ = ShapeType::circle;
    }
    Circle(float r){
        this->r_ = r;
        this->shape_type_ = ShapeType::circle;
    }
    float get_r(){
        return this->r_;
    }
    void set_r(float r){
        this->r_ = r;
    }

    Shape* clone(){
        return new Circle(*this);
    }

protected:
    float r_;
};

class Rectangle : public Shape{
public:
    Rectangle(){
        this->shape_type_ = ShapeType::rect;
    }

    Shape* clone(){
        return new Rectangle(*this);
    }
};

class Collider
{
public:
    Collider() = delete;
    Collider(Shape* shape, int layer, std::function<void(Collider* other)> func, IEntity* entity);
    Collider(Shape* shape, int layer, IEntity* entity);
    Collider(const Collider& other);
    Collider& operator=(const Collider& other);
    void set_layer(int layer);
    void set_on_collide(std::function<void(Collider* other)> func);
    void run_on_collide(Collider* collider);
    void set_shape(Shape* shape);
    int get_layer() const;
    Shape* get_shape() const;
    void set_owner(IEntity* entity);
    IEntity* get_owner() const;
    ~Collider();

private:
    int layer_;
    std::function<void(Collider* other)> on_collide_;
    Shape* shape_;
    IEntity* owner_;
};