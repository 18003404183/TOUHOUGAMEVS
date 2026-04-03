#include <algorithm>
#include <type_traits>
#include"ColliderManager.h"
#include"Danmaku.h"
#include"IEntity.h"
#include"Config.h"

// 碰撞检测代码量经过双重分发估计会大量增长 所以之后考虑重构 通过更加彻底的模板元编程
void ColliderManager::check_pool_collisions(DanmakuPool* pool, int target_layer) {
    if (!pool) return;
    auto& raw_pool = pool->get_raw_pool();

    // 扩大四叉树边界以覆盖整个游戏区域
    // 注意：pos是中心点，half_wh是半宽高
    // 游戏区域应该是 1280x720 或类似的分辨率
    static Quadtree<int> bullet_tree(
        {MAIN_WINDOW_WIDTH / 2.0f, MAIN_WINDOW_HEIGHT / 2.0f}, 
        {MAIN_WINDOW_WIDTH / 2.0f, MAIN_WINDOW_HEIGHT / 2.0f}, 
        0
    );
    bullet_tree.clear(); // 清空上一帧的数据

    for (int i = 0; i < (int)raw_pool.size(); ++i) {
        if (!raw_pool[i].active) continue;
        
        const auto& prefab = pool->get_prefab(raw_pool[i].prefab_id);
        
        // 使用 visit 来处理不同类型的碰撞体插入
        float r = std::visit([](const auto& box) -> float {
            using BoxType = std::decay_t<decltype(box)>;
            if constexpr (std::is_same_v<BoxType, CircleHitbox>) return box.radius;
            else if constexpr (std::is_same_v<BoxType, RectHitbox>) return (std::max)(box.half_w, box.half_h);
            else if constexpr (std::is_same_v<BoxType, LaserHitbox>) return (std::max)(box.length, box.half_w);
            return 0.0f;
        }, prefab.hitbox);

        if (r <= 0.0f) continue;
        bullet_tree.insert(i, AABB{{raw_pool[i].pos.x, raw_pool[i].pos.y}, {r + 2.0f, r + 2.0f}});
    }

    for (auto& col_ptr : this->collider_list_) {
        Collider* target_col = col_ptr.get();
        IEntity* owner = target_col->get_owner();

        if (target_col->get_layer() == target_layer && owner && owner->is_active()) {
            
            Shape* shape = target_col->get_shape();
            if (shape->get_shape() == ShapeType::circle) {
                float target_radius = static_cast<Circle*>(shape)->get_r();
                glm::vec2 target_pos = owner->get_position();

                // 增加查询范围的缓冲区，确保能检测到附近的子弹
                // 使用更大的缓冲区以提高检测灵敏度
                AABB query_range{{target_pos.x, target_pos.y}, {target_radius + 30.0f, target_radius + 30.0f}};

                static std::vector<int> result_list;
                result_list.clear();
                bullet_tree.query(result_list, query_range);

                for (auto& bullet_potential : result_list) {
                    DanmakuData& bullet = raw_pool[bullet_potential];
                    if (!bullet.active) continue; // 必须检查，防止重复处理
                    
                    const auto& prefab = pool->get_prefab(bullet.prefab_id);
                    
                    bool is_hit = std::visit([&](const auto& box) -> bool{
                        using BoxType = std::decay_t<decltype(box)>;//底层通过编译器查表
                        // 编译器if is_same_v底层是通过模板匹配来实现的 模板会优先匹配两个相同的类型 如果类型相同 匹配相同类型的模板 返回1 不同 则匹配不同类型的模板 返回0
                        if constexpr (std::is_same_v<BoxType, CircleHitbox>){
                            return CollisionSolution::check_circle_circle(
                                target_pos, bullet.pos,
                                target_radius, box.radius
                            );
                        }
                        else if constexpr (std::is_same_v<BoxType, RectHitbox>){
                            return CollisionSolution::check_circle_rect();
                        }
                        else if constexpr (std::is_same_v<BoxType, LaserHitbox>){
                            return CollisionSolution::check_circle_rect();
                        }
                    }, prefab.hitbox);

                    if(is_hit){
                        pool->destroy_bullet(bullet);
                        this->collision_events_.push_back({target_col, nullptr});
                    }
                    //这里编译器会把prefab.hitbox传给box(函数参数) visit会检查prefab.hitbox到底是哪种类型 并且传给auto& box(自动类型推导导致的)
                    //编译期进行推断和处理 为对应类型生成对应代码 然后调表去执行
                    // virant在编译后 会在自己储存区域的前端存储一个数字 用于区分是哪种类型
                }
            }
        }
    }
}