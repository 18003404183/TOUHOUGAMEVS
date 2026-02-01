#pragma once
#include "Collider.h"

class CollisionSolver {
public:
    // 基础：点与圆距离平方检测（无需开方，极快）
    static bool CheckPointCircle(glm::vec2 p, glm::vec2 c, float r) {
        float dx = p.x - c.x;
        float dy = p.y - c.y;
        return (dx * dx + dy * dy) < (r * r);
    }

    // 圆与圆
    static bool CheckCircleCircle(glm::vec2 c1, float r1, glm::vec2 c2, float r2) {
        float rSum = r1 + r2;
        float dx = c1.x - c2.x;
        float dy = c1.y - c2.y;
        return (dx * dx + dy * dy) < (rSum * rSum);
    }

    // 可以在这里扩展 Circle vs AABB 等
};