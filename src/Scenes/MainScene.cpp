#include "MainScene.h"
#include "SceneManager.h"
#include "Animation.h"
#include "GameContext.h"
#include "ResourcesManager.h"
#include "clock.h"
#include "Danmaku.h"
#include <fstream> 
#include "json.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

using json = nlohmann::json; 

MainScene::MainScene() {
    IInput::get();
    std::cout << "MainScene Constructor" << std::endl;
}

MainScene::~MainScene() { 
    std::cout << "MainScene Destructor" << std::endl; 
    if (enemy_bullets) delete enemy_bullets; // 记得清理战场
}

void MainScene::on_enter() {
    ResourcesManager::getInstance()->loadScene(SceneType::MainMenu);
    ColliderManager::get_instance();
    
    this->enemy_bullets = new DanmakuPool(10000);

    uint16_t tex_id = ResourcesManager::getInstance()->register_danmaku_texture("resources\\8.png");
    
    TrajectoryTemplate traj;
    //以当前速度飞 0.5 秒
    traj.commands.push_back({DanmakuCmdType::WAIT, 0.5f, 0.0f});
    //平滑转弯 90 度，耗时 1.0 秒
    traj.commands.push_back({DanmakuCmdType::SMOOTH_TURN, 360.0f, 1.0f});
    // 猛烈加速，每秒加 400 速度，持续 2.0 秒
    //traj.commands.push_back({DanmakuCmdType::ACCELERATE, 400.0f, 2.0f});
    
    // 注册轨迹并拿到 ID
    uint16_t traj_id = this->enemy_bullets->register_trajectory(traj);

    // 4. 组装预制体 (Prefab)
    DanmakuPrefab dp;
    dp.texture_id = tex_id; 
    dp.base_size = {16.0f, 16.0f}; // 强制渲染大小为 16x16，你可以根据实际图片修改
    dp.base_scale = {1.0f, 1.0f};
    dp.shape_type = DanmakuShape::Circle;
    dp.base_radius = 5.0f; // 碰撞判定半径
    dp.default_traj_id = traj_id; // 绑定刚才写的酷炫轨迹！

    // 注册预制体并保存 ID 到成员变量，方便按键时调用
    this->test_prefab_id = this->enemy_bullets->register_prefab(dp);
    // ==========================================

    std::ifstream file("resources\\level1.json");
    if(!file.is_open()){
        std::cerr << "未打开文件"<<std::endl;
    }
    else{
        json level_data;
        file >> level_data;

        std::cout << "成功加载关卡" << level_data["level_name"].get<std::string>() << std::endl;
        
        EnemyBuilder enemybuilder;

        for(const auto& wave : level_data["waves"]){
            for (const auto& enemy_data : wave["enemies"]) {
                
                // 读取 JSON 中的字符串类型，并转换为枚举
                std::string type_str = enemy_data["type"].get<std::string>();
                EnemyType type = EnemyType::common; // 默认给个 common
                
                if (type_str == "small") type = EnemyType::small;
                else if (type_str == "common") type = EnemyType::common;
                else if (type_str == "large") type = EnemyType::large;

                // 使用你的工厂生成基础敌人
                enemybuilder.create_enemy(type);
                Enemy* e = enemybuilder.create();

                // 🌟 核心点：用 JSON 里的数据覆盖默认数据！
                float x = enemy_data["x"].get<float>();
                float y = enemy_data["y"].get<float>();
                int hp = enemy_data["hp"].get<int>();

                e->set_position({x, y});
                e->setHp(hp);

                // 挂载到场景中
                this->entities.push_back(std::make_unique<IEntity>(e));
                this->renderables.push_back(e);
                this->updateables.push_back(e);
            }
        }
    }




    EventManager::get_instance()->subscribe(EventType::PlayerDead,[](const Event& event){
        
        if(event.data){
            int t = *static_cast<int*>(event.data);
            std::cout<<"收到事件玩家死亡"<<"收到数值"<< t <<"\n";
        }
    });

    this->clock.set_callback([this]() {
        std::cout << "Timer tick. this:" << this << std::endl; 
    });

    Texture *v = ResourcesManager::getInstance()->get_texture("resources\\2.png");
    glm::vec2 pos(205, 205);
    glm::vec2 scale(1, 1);

    Image player_image = Image(v, pos, scale, 0, 255);
    Atlas *player_atlas = ResourcesManager::getInstance()->get_atlas("resources\\6.png");

    Context::Instances()->set_player_context(pos, {0, 0}, true, true, 100);
    PlayerContext pc = Context::Instances()->get_player_context();
    this->player = new Player(pos,{0,0},100);

    player->setImage(player_image);
    Animation animation(player_atlas, 0.1);

    Collider* player_collider = ColliderManager::get_instance()->create_collider(new Circle(10),0,player);

    this->renderables.push_back(player);
    this->updateables.push_back(player);
    this->player->set_animation(animation);
    player->set_collider(player_collider);
    player->get_collider()->set_on_collid([p = this->player](Collider* other){
        p->set_alive(false);
        std::cout << "Player Dead!" << std::endl; 
    });
}

void MainScene::on_exit() { 
    std::cout << "Exiting MainScene" << std::endl;
    Context::Instances()->set_player_context(
        player->get_position(), player->get_velocity(), player->isActive(),
        player->is_alive(), player->get_hp());
}

void MainScene::on_update(float deltatime) {
    for (IUpdateable *a : this->updateables) {
        a->update(deltatime);
    }
    this->clock.update(deltatime);
    ImGui::Begin("danmaku count");
    ImGui::Text("danmaku: %d",this->enemy_bullets->count);
    ImGui::End();

    //IInput::update();

    ColliderManager::get_instance()->check_and_resolve_collisions();


    if (this->enemy_bullets) {
        this->enemy_bullets->update(deltatime);
        this->enemy_bullets->check_player_hit(this->player);
    }

}

void MainScene::on_render(SDLRender &renderer) {
    for (IRenderable *a : this->renderables) {
        a->render(renderer);
    }
    
    // ==========================================
    // 弹幕池极速渲染 (基于享元模式)
    // ==========================================
    if (this->enemy_bullets) {
        this->enemy_bullets->render(renderer);
    }
}

void MainScene::on_input() {
    if (IInput::get_key(KeyType::ENTER)->get_keydown())
        SceneManager::getInstance()->switchScene(SceneType::EndMenu);
    
    if (IInput::get_key(KeyType::RIGHT)->get_state()) // 改为 get_state() 支持长按平滑移动
        this->player->set_position(this->player->get_position() + glm::vec2{5, 0});
    if (IInput::get_key(KeyType::LEFT)->get_state())
        this->player->set_position(this->player->get_position() + glm::vec2{-5, 0});
    if (IInput::get_key(KeyType::DOWN)->get_state())
        this->player->set_position(this->player->get_position() + glm::vec2{0, 5});
    if (IInput::get_key(KeyType::UP)->get_state())
        this->player->set_position(this->player->get_position() + glm::vec2{0, -5});

    // ==========================================
    // 开火测试：按住 Z 键释放华丽的几何弹幕
    // ==========================================
    if (IInput::get_key(KeyType::Z)->get_state()) {
        // 东方经典设计：一帧发射一圈子弹 (例如 12 颗)
        for (int i = 0; i < 50; i++) {
            // 计算发射角度 (转为弧度)
            float angle = i * (360.0f / 12.0f) * (3.14159f / 180.0f);
            
            // 从屏幕中心 (400, 300) 发射，初速度 150
            this->enemy_bullets->spawn(this->test_prefab_id, {400, 300}, 150.0f, angle);
        }
    }

        // 在 on_input() 中添加：
    if (IInput::get_key(KeyType::C)->get_keydown()) {
        static int test_val = 408; // 随便搞个测试数据
        Event e = { EventType::PlayerDead, &test_val };
        
        std::cout << "[Input] 玩家按下了 C 键，准备发布事件..." << std::endl;
        EventManager::get_instance()->publish(e);
}
}