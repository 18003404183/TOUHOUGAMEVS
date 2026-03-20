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
}

void MainScene::on_enter() {
    ResourcesManager::getInstance()->loadScene(SceneType::MainMenu);
    ColliderManager::get_instance();
    
    this->enemy_bullets = std::make_unique<DanmakuPool>(10000);

    uint16_t tex_id = ResourcesManager::getInstance()->register_danmaku_texture("resources\\8.png");
    
    TrajectoryTemplate traj;
    //以当前速度飞 0.5 秒
    traj.commands.push_back({DanmakuCmdType::WAIT, 0.5f, 0.0f});
    //平滑转弯 90 度，耗时 1.0 秒
    traj.commands.push_back({DanmakuCmdType::SMOOTH_TURN, 360.0f, 1.0f});
    // 猛烈加速，每秒加 400 速度，持续 2.0 秒
    traj.commands.push_back({DanmakuCmdType::ACCELERATE, 400.0f, 2.0f});
    
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
        Texture *v = ResourcesManager::getInstance()->get_texture("resources\\2.png");
    glm::vec2 pos(500,500);
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
        Event e;
        int* a = new int(0); 
        e.data = a;
        e.type = EventType::PlayerDead;
        EventManager::get_instance()->publish(e);
        std::cout << "Player Dead!" << std::endl; 
    });

    this->entities.push_back(std::unique_ptr<IEntity>(this->player));

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

                enemybuilder.create_enemy(type);
                Enemy* e = enemybuilder.create();

                float x = enemy_data["x"].get<float>();
                float y = enemy_data["y"].get<float>();
                int hp = enemy_data["hp"].get<int>();

                e->set_position({x, y});
                e->setHp(hp);
                e->set_bullet_pool(this->enemy_bullets.get());

                // 👇 2. 组装“三向自机狙”散弹武器！
                // 定义扩散角度：15 度 (记得转成弧度)
                float spread_angle = 15.0f * (3.14159f / 180.0f);

                // 枪管 A：正中心 (绝对锁定玩家，逼迫玩家走位)
                e->add_emitter(std::make_unique<Emitter>(
                    EmitterSpace::Relative, e->get_position_ptr(), glm::vec2{0, 0},
                    AimMode::TargetAlways, this->player->get_position_ptr(),
                    this->test_prefab_id, 300.0f, 0.0f, 0.0f,
                    1.5f, -1
                ));

                // 枪管 B：左侧封位 (在锁定角度的基础上，向左偏 15 度)
                e->add_emitter(std::make_unique<Emitter>(
                    EmitterSpace::Relative, e->get_position_ptr(), glm::vec2{0, 0},
                    AimMode::TargetAlways, this->player->get_position_ptr(),
                    this->test_prefab_id, 300.0f, -spread_angle, 0.0f,
                    1.5f, -1
                ));

                // 枪管 C：右侧封位 (在锁定角度的基础上，向右偏 15 度)
                e->add_emitter(std::make_unique<Emitter>(
                    EmitterSpace::Relative, e->get_position_ptr(), glm::vec2{0, 0},
                    AimMode::TargetAlways, this->player->get_position_ptr(),
                    this->test_prefab_id, 300.0f, spread_angle, 0.0f,
                    1.5f, -1
                ));

                // 挂载到场景中
                this->entities.push_back(std::unique_ptr<IEntity>(e));//不能够使用make_unique来创建 因为IEntity是虚基类 无法实例化 可以先创建裸指针 然后再用unique_ptr接管
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
    ImGui::Begin("Engine Control Panel");
    
    if (ImGui::CollapsingHeader("Danmaku Stats", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Active Enemy Bullets: %d / 10000", this->enemy_bullets->count);
        // 进度条直观显示弹幕池压力
        ImGui::ProgressBar((float)this->enemy_bullets->count / 10000.0f);
    }

    if (ImGui::CollapsingHeader("Debug Tools", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Show Hitboxes (Debug Draw)", &show_debug_hitbox);
        if (ImGui::Button("Clear All Bullets")) {
            this->enemy_bullets.get()->clear_pool();
        }
    }

    if (ImGui::CollapsingHeader("Emitter Tweaker (Press Z)", ImGuiTreeNodeFlags_DefaultOpen)) {
        // 实时拖动滑块，改变按 Z 发射的子弹数量和速度！
        ImGui::SliderInt("Emit Count", &debug_emit_count, 1, 100);
        ImGui::SliderFloat("Emit Speed", &debug_emit_speed, 10.0f, 500.0f);
    }
    
    ImGui::End();
    //IInput::update();
    if (this->enemy_bullets) {
        this->enemy_bullets->update(deltatime);
    }
    // 检测碰撞

    ColliderManager::get_instance()->check_pool_collisions(this->enemy_bullets.get(),this->player->get_collider()->get_layer());
    ColliderManager::get_instance()->check_and_resolve_collisions();


}

void MainScene::on_render(SDLRender &renderer) {
    for (IRenderable *a : this->renderables) {
        a->render(renderer);
    }
    
    if (this->enemy_bullets) {
        this->enemy_bullets->render(renderer);
    }
    // 👇 2. 如果开启了调试模式，绘制所有的碰撞盒！
    if (this->show_debug_hitbox) {
        // 画玩家的核心判定点 (绿圈)
        if (this->player && this->player->is_alive()) {
            renderer.draw_circle_outline(this->player->get_position(), 10.0f, 0, 255, 0); // 半径2
        }

        // 画敌人的碰撞框 (蓝圈)
        for (auto& entity : this->entities) {
            Enemy* e = dynamic_cast<Enemy*>(entity.get());
            if (e && e->isAlive()) {
                // 你在 EnemyBuilder 里给了圆圈半径 500 (这可能太大了，你后续可能要调小)
                renderer.draw_circle_outline(e->get_position(), 30.0f, 0, 0, 255); 
            }
        }

        for (auto& d : this->enemy_bullets->get_raw_pool()) {
            if (!d.active) continue;
            float r = this->enemy_bullets->get_prefab(d.prefab_id).base_radius;
            renderer.draw_circle_outline(d.pos, r, 255, 0, 0); 
        }


    }
}

static int cooldown = 0;
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

    if (IInput::get_key(KeyType::Z)->get_state()) {
        // 控制一下发射频率，不然一按 60 帧每帧都发射就卡爆了
        if (cooldown <= 0) {
            for (int i = 0; i < debug_emit_count; i++) {
                float angle = i * (360.0f / debug_emit_count) * (3.14159f / 180.0f);
                // 使用我们在 ImGui 里调的初速度 debug_emit_speed
                this->enemy_bullets->spawn(this->test_prefab_id, {400, 300}, debug_emit_speed, angle);
            }
            cooldown = 5; // 5帧发射一次
        }
    
    cooldown--;
    }else {
        cooldown = 0;
    }
}

// 接下来准备写发射器逻辑 负责支持按照一些预制轨迹 在指定时间空间 生成弹幕(拥有一个update(float delta)来控制弹幕释放的时机) 并且支持将弹幕的释放逻辑外接给lua 支持绝对位置与绑定者的相对位置两种位置方式