#pragma once

#include<glm.hpp>

struct AABB
{
    glm::vec2 pos;
    glm::vec2 half_wh;

    bool intersects(const AABB& other) const {
        // 考虑性能更高
        if(std::abs(pos.x - other.pos.x) > half_wh.x + other.half_wh.x){
            return false;
        }
        if(std::abs(pos.y - other.pos.y) > half_wh.y + other.half_wh.y){
            return false;
        }
        return true;
    }
};
