#include<vector>
#include"Player.h"
#include"Enemy.h"
#include"Danmaku.h"
#include "Collider.h"
#include<unordered_map>

using collision_func = std::function<void()>;

class ColliderManager{
public:
    std::vector<Player*> player_collider_list;
    std::vector<Enemy*> enemy_collider_list;

    static ColliderManager* get_instance();
    //检测碰撞从所有队列中
    void check_collider();
    //执行已经碰撞的碰撞逻辑
    void execute_collider_logic();
    // 向列表中注册一个新的碰撞(包括碰撞形状与碰撞逻辑还有碰撞体名称)
    void register_collider(Shape* shape,collision_func func,std::string name);
    

private:
    ColliderManager() = default;
    static ColliderManager* collidermanager;

    std::vector<Shape*> shape_list;
    std::unordered_map<std::string,int> shape_id_map;
    std::unordered_map<std::string,int> func_id_map;
    std::vector<collision_func> func;

};