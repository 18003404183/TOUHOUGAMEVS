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

class ColliderManager{
public:


    static ColliderManager* get_instance(){
        static ColliderManager instance;
        return &instance;
    }

    // Collider& create_collider(){
    //     auto new_collider = std::make_unique<Collider>();
    //     auto& collider = *new_collider;
    //     collider_list.push_back(std::move(new_collider));
    //     return collider;
    // }

    Collider* create_collider(Shape* shape, int layer,IEntity* owner){
        auto new_collider = std::make_unique<Collider>(shape,layer,owner);
        auto collider = new_collider.get();
        collider_list.push_back(std::move(new_collider));
        return collider;
    }

    void destory_collider(Collider* collider){
        if(!collider) return;

        for(auto i = 0;i<collider_list.size();i++){
            if(collider_list[i].get() == collider){
                std::swap(collider_list[i],collider_list.back());
                collider_list.pop_back();
                return;
            }
        }
    }

    void check_and_resolve_collisions(){
        // 清除事件列表
        // 循环判断是否碰撞 将每个碰撞体向下转型成type标志的类型 接着调用碰撞算法库里面的函数 将所有检测到的碰撞产生一个碰撞事件并且存入队列
        for(size_t i = 0; i < this->collider_list.size(); ++i){
            for(size_t j = i + 1; j < this->collider_list.size(); ++j){
                Collider* c1 = this->collider_list[i].get();
                Collider* c2 = this->collider_list[j].get();//这里裸指针不会删除资源
                if(c1->get_layer() == c2->get_layer()) continue;
                if(CollisionSolution::CheckCollision(c1,c2)){
                    this->collision_events.push_back({c1,c2});

                }
            }
        }
        //对于队列里的每一个碰撞体 确认存在碰撞函数后执行碰撞回调函数
        for(auto i : this->collision_events){
            if(i.collider_object1){
                i.collider_object1->run_on_collid(i.collider_object2);
            }
            if(i.collider_object2){
                i.collider_object2->run_on_collid(i.collider_object1);
            }
        }
        this->collision_events.clear();       
    }

    void check_pool_collisions(DanmakuPool* pool, int target_layer);

    ~ColliderManager(){
        this->collider_list.clear();
        this->collision_events.clear();
    };


private:
    ColliderManager() = default;

    static ColliderManager* collider_manager;

    std::vector<std::unique_ptr<Collider>> collider_list;
    std::vector<CollisionEvent> collision_events;

};