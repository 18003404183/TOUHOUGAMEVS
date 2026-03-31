#pragma once

#include "Collider.h"

class CollisionSolution
{
public:
    static bool check_collision(Collider* a, Collider* b)
    {
        ShapeType type_a = a->get_shape()->get_shape();
        ShapeType type_b = b->get_shape()->get_shape();

        if (type_a == ShapeType::circle && type_b == ShapeType::circle)
        {
            Circle* c1 = static_cast<Circle*>(a->get_shape());
            Circle* c2 = static_cast<Circle*>(b->get_shape());
            return check_circle_circle(
                a->get_owner()->get_position(),
                b->get_owner()->get_position(),
                c1->get_r(),
                c2->get_r());
        }

        return false;
    }

    static bool check_circle_circle(glm::vec2 pos1, glm::vec2 pos2, float r1, float r2)
    {
        float x = pos1.x - pos2.x;
        float y = pos1.y - pos2.y;
        return x * x + y * y <= (r1 + r2) * (r1 + r2);
    }

    static bool check_circle_rect()
    {
        return false;
    }

    static bool check_rect_rect()
    {
        return false;
    }
};