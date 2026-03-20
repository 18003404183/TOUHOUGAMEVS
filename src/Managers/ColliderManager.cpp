#include"ColliderManager.h"
#include"Danmaku.h"
#include"IEntity.h"

void ColliderManager::check_pool_collisions(DanmakuPool* pool, int target_layer) {
    if (!pool) return;
    auto& raw_pool = pool->get_raw_pool();

    // 1. 遍历管家自己管理的所有碰撞体！根本不用去碰 Entity 列表！
    for (auto& col_ptr : this->collider_list) {
        Collider* target_col = col_ptr.get();
        IEntity* owner = target_col->get_owner();

        // 🎯 核心过滤：只检测符合目标层级的碰撞体，并且它的主人必须还活着
        if (target_col->get_layer() == target_layer && owner && owner->isActive()) {
            
            Shape* shape = target_col->get_shape();
            if (shape->get_shape() == ShapeType::circle) {
                float target_radius = static_cast<Circle*>(shape)->get_r();
                glm::vec2 target_pos = owner->get_position();

                // 2. 遍历弹幕池 (DOD 极速平推)
                for (auto& bullet : raw_pool) {
                    if (!bullet.active) continue;

                    float bullet_radius = pool->get_prefab(bullet.prefab_id).base_radius;

                    if (CollisionSolution::CheckCircleCircle(bullet.pos, target_pos, bullet_radius, target_radius)) {
                        
                        pool->destroy_bullet(bullet);
                        
                        // 产生单边事件：只通知被撞的碰撞体，另一个传 nullptr 代表子弹
                        this->collision_events.push_back({target_col, nullptr});
                    }
                }
            }
        }
    }
}