#include<ColliderManager.h>
#include"CollisionSolver.h"

ColliderManager* ColliderManager::collidermanager = nullptr;

ColliderManager* ColliderManager::get_instance(){
    if(!collidermanager){
        collidermanager = new ColliderManager;
    }

    return collidermanager;
}

void ColliderManager::check_collider(){
    //敌人与玩家碰撞
    for(Player* p : this->player_collider_list){
        for(Enemy* e : this->enemy_collider_list){
            auto shape1 = static_cast<const CircleShape*>(p->get_shape());
            auto shape2 = static_cast<const CircleShape*>(e->get_shape());

            if(CollisionSolver::CheckCircleCircle(e->get_position(),shape2->radius,p->get_position(),shape1->radius)){
                p->set_is_collision(true);
                e->set_is_collision(true);
            }
        }
    }
}

void ColliderManager::execute_collider_logic(){
    for(Player* p : this->player_collider_list){
        if(!p->get_is_collision()) return;
        else{
            p->on_collision();
            p->set_is_collision(false);
        }
    }
    for(Enemy* e : this->enemy_collider_list){
        if(!e->get_is_collision()){
            return;
        }
        else{
            e->on_collision();
            e->set_is_collision(false);
        }
    }
}

void ColliderManager::register_collider(Shape* shape,collision_func func,std::string){

}