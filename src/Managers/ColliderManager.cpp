#include"ColliderManager.h"
#include"Danmaku.h"
#include"IEntity.h"

// 碰撞检测代码量经过双重分发估计会大量增长 所以之后考虑重构 通过更加彻底的模板元编程
void ColliderManager::check_pool_collisions(DanmakuPool* pool, int target_layer) {
    if (!pool) return;
    auto& raw_pool = pool->get_raw_pool();

    for (auto& col_ptr : this->collider_list) {
        Collider* target_col = col_ptr.get();
        IEntity* owner = target_col->get_owner();

        if (target_col->get_layer() == target_layer && owner && owner->isActive()) {
            
            Shape* shape = target_col->get_shape();
            if (shape->get_shape() == ShapeType::circle) {
                float target_radius = static_cast<Circle*>(shape)->get_r();
                glm::vec2 target_pos = owner->get_position();

                for (auto& bullet : raw_pool) {
                    if (!bullet.active) continue;
                    const auto& prefab = pool->get_prefab(bullet.prefab_id);
                    
                    bool is_hit = std::visit([&](const auto& box) -> bool{
                        using BoxType = std::decay_t<decltype(box)>;//底层通过编译器查表
                        // 编译器if is_same_v底层是通过模板匹配来实现的 模板会优先匹配两个相同的类型 如果类型相同 匹配相同类型的模板 返回1 不同 则匹配不同类型的模板 返回0
                        if constexpr (std::is_same_v<BoxType,CircleHitbox>){
                            return CollisionSolution::CheckCircleCircle(
                                target_pos,bullet.pos,
                                target_radius,box.radius
                            );
                        }
                        else if constexpr (std::is_same_v<BoxType,RectHitbox>){
                            return CollisionSolution::CheckCircleRect();
                        }
                        else if constexpr (std::is_same_v<BoxType,LaserHitbox>){
                            return CollisionSolution::CheckCircleRect();
                        }
                    },prefab.hitbox);

                    if(is_hit){
                        pool->destroy_bullet(bullet);
                        this->collision_events.push_back({target_col,nullptr});
                    }
                    //这里编译器会把prefab.hitbox传给box(函数参数) visit会检查prefab.hitbox到底是哪种类型 并且传给auto& box(自动类型推导导致的)
                    //编译期进行推断和处理 为对应类型生成对应代码 然后调表去执行
                    // virant在编译后 会在自己储存区域的前端存储一个数字 用于区分是哪种类型
                }
            }
        }
    }
}