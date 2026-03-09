#include"Collider.h"


Collider::Collider(Shape *shape, int layer, std::function<void(Collider* other)> func,IEntity* owner)
{
    assert(this->shape != nullptr && "Collider initialization failed: Shape cannot be null!");
    this->shape = shape;
    this->layer = layer;
    this->on_collid = func;
    this->owner = owner;

}

Collider::Collider(Shape *shape, int layer,IEntity* owner)
{
    assert(this->shape != nullptr && "Collider initialization failed: Shape cannot be null!");
    this->layer = layer;
    this->owner = owner;
    this->shape = shape->clone();
}

Collider::Collider(const Collider &other)
{
    this->layer = other.layer;

    this->shape = shape->clone();

}

Collider Collider::operator=(const Collider &other)
{
    this->layer = other.layer;
    this->shape = other.shape->clone();
    return Collider(shape->clone(),this->layer,nullptr);
}

void Collider::set_layer(int layer)
{
    this->layer = layer;
}

void Collider::set_on_collid(std::function<void(Collider* other)> func)
{
    this->on_collid = func;
}

void Collider::run_on_collid(Collider *collider)
{
    this->on_collid(collider);
}

void Collider::set_shape(Shape *shape)
{
    if(!shape) return;
    if(this->shape == shape) return;
    delete this->shape;
    this->shape = shape->clone();
}

int Collider::get_layer()
{

    return this->layer;
}

Shape *Collider::get_shape()
{
    return this->shape;
}

void Collider::set_owner(IEntity* entity){
    this->owner = entity;
}

IEntity *Collider::get_owner()
{
    return this->owner;
}

Collider::~Collider()
{
    delete this->shape;
}
