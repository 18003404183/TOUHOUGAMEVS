#pragma once

#include"CollisionSolver.h"
#include"Collider.h"
#include"unordered_map"
#include<vector>
#include<memory>
#include"Danmaku.h"
#include"Quadtree.h"


struct CollisionEvent{
    Collider* collider_object1;
    Collider* collider_object2; 
};

class ColliderManager
{
public:
    static ColliderManager* get_instance()
    {
        static ColliderManager instance;
        return &instance;
    }

    Collider* create_collider(Shape* shape, int layer, IEntity* owner)
    {
        auto new_collider = std::make_unique<Collider>(shape, layer, owner);
        auto collider = new_collider.get();
        collider_list_.push_back(std::move(new_collider));
        return collider;
    }

    void destroy_collider(Collider* collider)
    {
        if (!collider) return;

        for (size_t i = 0; i < collider_list_.size(); i++)
        {
            if (collider_list_[i].get() == collider)
            {
                std::swap(collider_list_[i], collider_list_.back());
                collider_list_.pop_back();
                return;
            }
        }
    }

    void check_and_resolve_collisions()
    {
        for (size_t i = 0; i < collider_list_.size(); ++i)
        {
            for (size_t j = i + 1; j < collider_list_.size(); ++j)
            {
                Collider* c1 = collider_list_[i].get();
                Collider* c2 = collider_list_[j].get();
                if (c1->get_layer() == c2->get_layer()) continue;
                if (CollisionSolution::check_collision(c1, c2))
                {
                    collision_events_.push_back({c1, c2});
                }
            }
        }

        for (auto& event : collision_events_)
        {
            if (event.collider_object1)
            {
                event.collider_object1->run_on_collide(event.collider_object2);
            }
            if (event.collider_object2)
            {
                event.collider_object2->run_on_collide(event.collider_object1);
            }
        }
        collision_events_.clear();
    }

    void check_pool_collisions(DanmakuPool* pool, int target_layer);

    ~ColliderManager()
    {
        collider_list_.clear();
        collision_events_.clear();
    }

private:
    ColliderManager() = default;

    std::vector<std::unique_ptr<Collider>> collider_list_;
    std::vector<CollisionEvent> collision_events_;
};