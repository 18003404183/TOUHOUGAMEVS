#pragma once

#include"Collider.h"

class CollisionSolution{
public:

    static bool CheckCollision(Collider* a,Collider* b){
        ShapeType typeA = a->get_shape()->get_shape();
        ShapeType typeB = b->get_shape()->get_shape();

        if(typeA == ShapeType::circle && typeB == ShapeType::circle){
            Circle* c1 = static_cast<Circle*>(a->get_shape());
            Circle* c2 = static_cast<Circle*>(b->get_shape());
            return CheckCircleCircle(
                a->get_owner()->get_position(),
                b->get_owner()->get_position(),
                c1->get_r(),
                c2->get_r()      
        );
        }

        return false;
    }

    static bool CheckCircleCircle(glm::vec2 pos1,glm::vec2 pos2,float r1,float r2){
        float x = pos1.x - pos2.x;
        float y = pos1.y - pos2.y;
        return x*x + y*y <= (r1 + r2)*(r1 + r2);
    }

    static bool CheckCircleRect(){
        return false;
    }

    static bool CheckRectRect(){
        return false;
    }




};

