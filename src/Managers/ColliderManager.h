#pragma once

#include"CollisionSolver.h"
#include"Collider.h"
#include"unordered_map"
#include<vector>
#include<memory>

struct CollisionEvent{
    Collider* collider_object1;
    Collider* collider_object2; 
};

class ColliderManager{
public:


    static ColliderManager* get_instance(){
        if(collider_manager == nullptr){
            collider_manager = new ColliderManager;
        }
        return collider_manager;
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

    void destory_collider(Collider& collider){
        for(auto i = collider_list.begin();i!=collider_list.end();i++){
            if((*i).get() == &collider){
                collider_list.erase(i);
                return;
            }
        }
    }

    void check_and_resolve_collisions(){
        // 清除事件列表
        this->collision_events.clear();       
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
            i.collider_object1->run_on_collid(i.collider_object2);
            i.collider_object2->run_on_collid(i.collider_object1);
        }
    }

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