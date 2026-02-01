#include<ColliderManager.h>

ColliderManager* ColliderManager::collidermanager = nullptr;

ColliderManager* ColliderManager::get_instance(){
    if(!collidermanager){
        collidermanager = new ColliderManager;
    }

    return collidermanager;
}

void ColliderManager::check_collider(){

}

void ColliderManager::execute_collider_logic(){

}

void ColliderManager::register_collider(Shape* shape,collision_func func,std::string){
    
}