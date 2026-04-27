#include"EnemyFactory.h"
#include<fstream>
#include"Enemy.h"
#include"EventManager.h"
#include"Image.h"

void EnemyFactory::load_json(const std::string& filepath){

    std::ifstream file(filepath);
    if(!file.is_open()){
        std::cerr << "找不到文件" << filepath << std::endl;
    }

    json j;
    
    try{
        file >> j;
    }
    catch(const json::parse_error& e){
        std::cerr << "json语法错误" << e.what() << std::endl;
        return;
    }

    // 看一个json数据是否包含 类型识别属性 如果没有 或者 类型不同直接退出
    if(j.contains("_file_type")){
        if(j["_file_type"] != "enemy_database"){
            std::cerr << "错误 传入错误的json配置 敌人工厂拒绝解析"<<std::endl;
            return;
        }
    }
    else{
        std::cerr << "未知的josn文件 不包含解析类型" << std::endl;
    }

    // 遍历每一个敌人类型 注册进工厂
    for(auto& [enemy_name,enemy_data] : j["enemies"].items()){
        EnemyConfig config;
        config.hp_           = enemy_data.value("hp",10.0f);
        config.image_name_   = enemy_data.value("image_name","");
        config.script_path_ = enemy_data.value("script_path","");
        if(enemy_data.contains("velocity")){
            config.velocity_ =  parse_vec2(enemy_data["velocity"]);
        }
        else{
            config.velocity_ = {0.0f,0.0f};
        }
        if(enemy_data.contains("animation")){
            config.animation_ = parse_animation_config(enemy_data["animation"]);   
        }
        if(enemy_data.contains("emitters")){
            for(auto& [emitter_name,emitter_data] : enemy_data["emitters"].items()){
                config.emitters_[emitter_name] = parse_Emitter_config(emitter_data);
            }
        }

        prototypes_[enemy_name] = config;
        std::cout << "[enemy_factory] add enemy prototypes:" << enemy_name << std::endl;  
    }   

}

//更具配置名称拿到一个对象 位置数据需要自己填写
std::unique_ptr<Enemy> EnemyFactory::build_enemy(const std::string& enemy_name, float x, float y) {
    // 1. 在原型库中查找对应的配置
    auto it = prototypes_.find(enemy_name);
    if (it == prototypes_.end()) {
        std::cerr << "[EnemyFactory] 错误：找不到指定的敌人原型 -> " << enemy_name << std::endl;
        return nullptr; 
    }
    const EnemyConfig& config = it->second;

    // 2. 创建基础实体
    auto enemy = std::make_unique<Enemy>();
    
    // 3. 注入基础数值
    enemy->set_position({x, y});
    enemy->set_velocity(config.velocity_);
    enemy->set_hp(config.hp_);
    enemy->set_active(true);
    enemy->set_alive(true);

    // 4. 装配视觉资源 (Texture & Animation)
    auto res_mgr = ResourcesManager::get_instance();
    
    if (!config.image_name_.empty()) {
        Texture* tex = res_mgr->get_texture(config.image_name_);
        if (tex) {
            enemy->set_image(Image(tex, {x, y}, {1, 1}, 0, 255));
        }
    }

    if (!config.animation_.atlas_name_.empty()) {
        Atlas* atlas = res_mgr->get_atlas(config.animation_.atlas_name_);
        if (atlas) {
            Animation anim(atlas, config.animation_.frame_speed_);
            enemy->set_animation(anim);
        }
    }

    // 5. 装配碰撞体 (按照你注释的要求，目前统一写死逻辑)
    Collider* collider = ColliderManager::get_instance()->create_collider(new Circle(30), 1, enemy.get());
    enemy->collider_ = collider;
    
    // 绑定受击与死亡逻辑
    enemy->collider_->set_on_collide([e = enemy.get()](Collider* other) {
        e->take_damage(10); // 暂时写死受击伤害为10
        if(e->get_hp() <= 0 && e->is_alive()){
            e->set_alive(false);
            
            Event ev;
            ev.type = EventType::EnemyDsetroyed;
            ev.data = e;
            EventManager::get_instance()->publish(ev);
        }		
    });

    // 6. 挂载弹幕发射器 (Emitters)
    for (const auto& [emitter_name, e_config] : config.emitters_) {
        
        // 字符串转枚举
        AimMode mode = AimMode::Fixed;
        if (e_config.aim_mode_ == "TargetInitial") mode = AimMode::TargetInitial;
        else if (e_config.aim_mode_ == "TargetAlways") mode = AimMode::TargetAlways;

        // 【注意】这里传入的 Target 指针暂时给 nullptr
        // 因为工厂不知道玩家在哪里。你需要在外部 (比如 Scene 或通过全局 Context) 绑定玩家位置。
        const glm::vec2* target_ptr = nullptr; 

        // 角度转换：JSON 中通常填角度 (如 15 度)，C++ 计算需要弧度
        float rad_base = e_config.base_angle_ * (3.14159265f / 180.0f);
        float rad_step = e_config.angle_step_ * (3.14159265f / 180.0f);

        auto emitter = std::make_unique<Emitter>(
            EmitterSpace::Relative, 
            enemy->get_position_ptr(), // 绑定到敌人当前位置
            e_config.offset_,          // 枪管偏移
            mode, 
            target_ptr,                // 索敌目标 (待绑定)
            e_config.prefab_id_, 
            e_config.speed_, 
            rad_base, 
            rad_step, 
            e_config.interval_, 
            e_config.count_
        );
        
        enemy->add_emitter(std::move(emitter));
    }

    // 7. 脚本绑定 (如果有的话)
    if (!config.script_path_.empty()) {
        // TODO: 你的 Enemy 类目前还没有挂载 Lua 脚本的成员变量
        // 未来可以在这里调用 enemy->set_script(config.script_path_)
        std::cout << "[EnemyFactory] 提示：实体 " << enemy_name << " 需要挂载脚本 " << config.script_path_ << "\n";
    }

    return enemy;
}

