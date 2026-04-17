#include "MainScene.h"
#include "SceneManager.h"
#include "Animation.h"
#include "GameContext.h"
#include "ResourcesManager.h"
#include "Clock.h"
#include "Danmaku.h"
#include <fstream> 
#include "json.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "BossScript.h"


using json = nlohmann::json; 

MainScene::MainScene() {
    IInput::get();
    std::cout << "MainScene Constructor" << std::endl;
}

MainScene::~MainScene() { 
    std::cout << "MainScene Destructor" << std::endl; 
}

void MainScene::on_enter()
{
    ResourcesManager::get_instance()->load_scene(SceneType::MainMenu);
    ColliderManager::get_instance();

    //测试Lua脚本
    this->boss_script_ = std::make_unique<BossScript>();
    this->boss_script_->init();

    enemy_bullets_ = std::make_unique<DanmakuPool>(10000);

    uint16_t tex_id = ResourcesManager::get_instance()->register_danmaku_texture("resources\\8.png");

    TrajectoryTemplate traj;
    //以当前速度飞 0.5 秒
    traj.commands.push_back({DanmakuCmdType::WAIT, 0.5f, 0.0f});
    //平滑转弯 90 度，耗时 1.0 秒
    traj.commands.push_back({DanmakuCmdType::SMOOTH_TURN, 360.0f, 1.0f});
    // 猛烈加速，每秒加 400 速度，持续 2.0 秒
    traj.commands.push_back({DanmakuCmdType::ACCELERATE, 400.0f, 2.0f});
    
    // 注册轨迹并拿到 ID
    uint16_t traj_id = DanmakuPool::register_trajectory(traj);

    // 4. 组装预制体 (Prefab)
    DanmakuPrefab dp;
    dp.texture_id = tex_id; 
    dp.base_size = {16,16}; // 强制渲染大小为 16x16，你可以根据实际图片修改
    dp.base_scale = {1,1};
    //dp.shape_type = DanmakuShape::Circle;
    //dp.base_radius = 5.0f; // 碰撞判定半径
    dp.default_traj_id = traj_id;
    dp.hitbox = CircleHitbox{5.0f};
    // 注册预制体并保存 ID 到成员变量，方便按键时调用
    this->test_prefab_id_ = DanmakuPool::register_prefab(dp);
    // ==========================================
    Texture* v = ResourcesManager::get_instance()->get_texture("resources\\2.png");
    glm::vec2 pos(500, 500);
    glm::vec2 scale(1, 1);

    Image player_image = Image(v, pos, scale, 0, 255);
    Atlas* player_atlas = ResourcesManager::get_instance()->get_atlas("resources\\6.png");

    Context::instances()->set_player_context(pos, {0, 0}, true, true, 100);
    PlayerContext pc = Context::instances()->get_player_context();
    player_ = new Player(pos, {0, 0}, 100);

    player_->set_image(player_image);
    Animation animation(player_atlas, 0.1);

    Collider* player_collider = ColliderManager::get_instance()->create_collider(new Circle(10), 0, player_);

    renderables_.push_back(player_);
    updateables_.push_back(player_);
    player_->set_animation(animation);
    player_->set_collider(player_collider);
    player_->get_collider()->set_on_collide([p = player_](Collider* other)
    {
        p->set_alive(false);
        Event e;
        int* a = new int(0);
        e.data = a;
        e.type = EventType::PlayerDead;
        EventManager::get_instance()->publish(e);
        std::cout << "Player Dead!" << std::endl;
    });
    player_->set_bullet_pool(std::make_unique<DanmakuPool>(10000));
    player_->add_emitter(std::make_unique<Emitter>(
        EmitterSpace::Relative, player_->get_position_ptr(), glm::vec2{-10, -5},
        AimMode::Fixed, nullptr,
        test_prefab_id_, 1200.0f, -3.14159f / 2.0f, 0.0f,
        0.1, -1
    ));

    player_->add_emitter(std::make_unique<Emitter>(
        EmitterSpace::Relative, player_->get_position_ptr(), glm::vec2{10, -5},
        AimMode::Fixed, nullptr,
        test_prefab_id_, 1200.0f, -3.14159f / 2.0f, 0.0f,
        0.066f, -1
    ));

    entities_.push_back(std::unique_ptr<IEntity>(player_));

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
                e->set_hp(hp);
                e->set_bullet_pool(enemy_bullets_.get());

                float spread_angle = 15.0f * (3.14159f / 180.0f);

                // 枪管 A：正中心 (绝对锁定玩家，逼迫玩家走位)
                e->add_emitter(std::make_unique<Emitter>(
                    EmitterSpace::Relative, e->get_position_ptr(), glm::vec2{0, 0},
                    AimMode::TargetAlways, player_->get_position_ptr(),
                    test_prefab_id_, 300.0f, 0.0f, 0.0f,
                    1.5f, -1
                ));

                // 枪管 B：左侧封位 (在锁定角度的基础上，向左偏 15 度)
                e->add_emitter(std::make_unique<Emitter>(
                    EmitterSpace::Relative, e->get_position_ptr(), glm::vec2{0, 0},
                    AimMode::TargetAlways, player_->get_position_ptr(),
                    test_prefab_id_, 300.0f, -spread_angle, 0.0f,
                    1.5f, -1
                ));

                // 枪管 C：右侧封位 (在锁定角度的基础上，向右偏 15 度)
                e->add_emitter(std::make_unique<Emitter>(
                    EmitterSpace::Relative, e->get_position_ptr(), glm::vec2{0, 0},
                    AimMode::TargetAlways, player_->get_position_ptr(),
                    test_prefab_id_, 300.0f, spread_angle, 0.0f,
                    1.5f, -1
                ));

                // 挂载到场景中
                entities_.push_back(std::unique_ptr<IEntity>(e));
                renderables_.push_back(e);
                updateables_.push_back(e);
            }
        }
    }




    EventManager::get_instance()->subscribe(EventType::PlayerDead,[](const Event& event){
        
        if(event.data){
            int t = *static_cast<int*>(event.data);
            std::cout<<"收到事件玩家死亡"<<"收到数值"<< t <<"\n";
        }
    });

    this->clock_.set_callback([this]() {
        std::cout << "Timer tick. this:" << this << std::endl; 
    });


}

void MainScene::on_exit()
{
    std::cout << "Exiting MainScene" << std::endl;
    Context::instances()->set_player_context(
        player_->get_position(), player_->get_velocity(), player_->is_active(),
        player_->is_alive(), player_->get_hp());
}

void MainScene::on_update(float delta_time)
{
    for (IUpdateable* a : updateables_)
    {
        a->update(delta_time);
    }
    clock_.update(delta_time);
    ImGui::Begin("Engine Control Panel");

    if (ImGui::CollapsingHeader("Danmaku Stats", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Active Enemy Bullets: %d / 10000", enemy_bullets_->count);
        ImGui::ProgressBar((float)enemy_bullets_->count / 10000.0f);
    }

    if (ImGui::CollapsingHeader("Debug Tools", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Show Hitboxes (Debug Draw)", &show_debug_hitbox_);
        if (ImGui::Button("Clear All Bullets"))
        {
            enemy_bullets_->clear_pool();
        }
    }

    if (ImGui::CollapsingHeader("Emitter Tweaker (Press Z)", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderInt("Emit Count", &debug_emit_count_, 1, 100);
        ImGui::SliderFloat("Emit Speed", &debug_emit_speed_, 10.0f, 500.0f);
    }

    ImGui::End();

    if (enemy_bullets_)
    {
        enemy_bullets_->update(delta_time);
    }

    ColliderManager::get_instance()->check_pool_collisions(enemy_bullets_.get(), player_->get_collider()->get_layer());
    if (player_ && player_->get_danmaku_pool())
    {
        ColliderManager::get_instance()->check_pool_collisions(player_->get_danmaku_pool(), 1);
    }
    ColliderManager::get_instance()->check_and_resolve_collisions();

    boss_script_.get()->update(delta_time);    
}

void MainScene::on_render(SDLRender& renderer)
{
    for (IRenderable* a : renderables_)
    {
        a->render(renderer);
    }

    if (enemy_bullets_)
    {
        enemy_bullets_->render(renderer);
    }

    if (show_debug_hitbox_)
    {
        if (player_ && player_->is_alive())
        {
            renderer.draw_circle_outline(player_->get_position(), 10.0f, 0, 255, 0);
        }

        for (auto& entity : entities_)
        {
            Enemy* e = dynamic_cast<Enemy*>(entity.get());
            if (e && e->is_alive())
            {
                renderer.draw_circle_outline(e->get_position(), 30.0f, 0, 0, 255);
            }
        }

        for (auto& d : enemy_bullets_->get_raw_pool())
        {
            if (!d.active) continue;
            if (const auto& pre = std::get_if<CircleHitbox>(&enemy_bullets_->get_prefab(d.prefab_id).hitbox))
            {
                renderer.draw_circle_outline(d.pos, pre->radius, 254, 0, 0);
            }
        }
        for(auto& d : player_->get_danmaku_pool()->get_raw_pool()){
            if (!d.active) continue;
            if (const auto& pre = std::get_if<CircleHitbox>(&enemy_bullets_->get_prefab(d.prefab_id).hitbox))
            {
                renderer.draw_circle_outline(d.pos, pre->radius, 254, 0, 0);
            }
        }
    }
}

static int cooldown = 0;

void MainScene::on_input()
{
    if (IInput::get_key(KeyType::ENTER)->get_keydown())
        SceneManager::get_instance()->switch_scene(SceneType::EndMenu);

    if (player_ && player_->is_active())
    {
        player_->handle_input();
    }


}

// 接下来准备写发射器逻辑 负责支持按照一些预制轨迹 在指定时间空间 生成弹幕(拥有一个update(float delta)来控制弹幕释放的时机) 并且支持将弹幕的释放逻辑外接给lua 支持绝对位置与绑定者的相对位置两种位置方式 - 3.20
// 准备实现一个场景 用来将整个流程过一遍 确保能够跑通
// 分为一下几个目标
// 1. 找到美术资源 搭建场景 至少需要 背景ui 弹幕与人物与字体的贴图 滚动背景的贴图 (其他的细节控件贴图日后再说)
// 2. 将整个关卡的流程改为尽可能外部数据驱动 比如哪个贴图放哪里 敌人如何生成 在哪生成 轨迹如何 发射什么样的子弹 boss何时出场 boss的舞台演出 交给外部的lua 与 json
// 3.搭建场景流程 舞台演出逻辑 比如什么时候刷什么敌人 打多少个敌人之后出boss boss有几个符卡 如何设计 这一步依赖于第二步
// 4.继续完善一些底层的功能 比如敌人死后生成p点 玩家随着p点越多 火力等级越高(weapon) 分数统计 玩家释放符卡等 - 3.25

// 准备重构弹幕碰撞 然后实现四×树来进行优化


//4.17
// 敌人会有很多种 至少得可以自定义 所以我感觉可以使用建造者+原型模式来管理敌人 而且最好我使用json就能定制敌人出来 所以我应该专门写一个enemy_Creater出来 然后读取json来创建敌人的原型 最后就能根据敌人原型的名字生成敌人
// 考虑到如果存储enemy对象 当涉及拷贝时会有许多的麻烦 所以考虑面向数据 敌人原型只存储敌人的属性(enemy_config) 然后在builder中构造敌人
