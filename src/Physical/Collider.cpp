#include "Collider.h"

Collider::Collider(Shape* shape, int layer, std::function<void(Collider* other)> func, IEntity* owner)
{
    assert(shape != nullptr && "Collider initialization failed: Shape cannot be null!");
    shape_ = shape;
    layer_ = layer;
    on_collide_ = func;
    owner_ = owner;
}

Collider::Collider(Shape* shape, int layer, IEntity* owner)
{
    assert(shape != nullptr && "Collider initialization failed: Shape cannot be null!");
    layer_ = layer;
    owner_ = owner;
    shape_ = shape;
}

Collider::Collider(const Collider& other)
{
    layer_ = other.layer_;
    shape_ = other.shape_->clone();
}

Collider& Collider::operator=(const Collider& other)
{
    layer_ = other.layer_;
    delete shape_;
    shape_ = other.shape_->clone();
    return *this;
}

void Collider::set_layer(int layer)
{
    layer_ = layer;
}

void Collider::set_on_collide(std::function<void(Collider* other)> func)
{
    on_collide_ = std::move(func);
}

void Collider::run_on_collide(Collider* collider)
{
    if (!on_collide_) return;
    on_collide_(collider);
}

void Collider::set_shape(Shape* shape)
{
    if (!shape) return;
    if (shape_ == shape) return;
    delete shape_;
    shape_ = shape;
}

int Collider::get_layer() const
{
    return layer_;
}

Shape* Collider::get_shape() const
{
    return shape_;
}

void Collider::set_owner(IEntity* entity)
{
    owner_ = entity;
}

IEntity* Collider::get_owner() const
{
    return owner_;
}

Collider::~Collider()
{
    delete shape_;
}