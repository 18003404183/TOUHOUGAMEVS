#include "Danmaku.h"
#include <cmath>

DanmakuPool::DanmakuPool(size_t pool_size) {
    // 一次性分配连续物理内存，杜绝 new/delete
    pool.resize(pool_size);
    for (auto& d : pool) {
        d.active = false;
    }
}

uint16_t DanmakuPool::register_prefab(const DanmakuPrefab& prefab) {
    prefabs.push_back(prefab);
    return static_cast<uint16_t>(prefabs.size() - 1);
}

uint16_t DanmakuPool::register_trajectory(const TrajectoryTemplate& traj) {
    trajectories.push_back(traj);
    return static_cast<uint16_t>(trajectories.size() - 1);
}

void DanmakuPool::spawn(uint16_t prefab_id, glm::vec2 start_pos, float initial_speed, float initial_angle) {
    if (prefab_id >= prefabs.size()) return;

    for (auto& d : pool) {
        if (!d.active) {
            d.active = true;
            d.prefab_id = prefab_id;
            d.pos = start_pos;
            d.current_speed = initial_speed;
            d.current_angle = initial_angle;
            
            // 绑定轨迹模板并重置状态机
            d.trajectory_id = prefabs[prefab_id].default_traj_id;
            d.current_cmd_index = 0;
            d.cmd_timer = 0.0f;
            
            // 初始化向量
            d.vel.x = std::cos(d.current_angle) * d.current_speed;
            d.vel.y = std::sin(d.current_angle) * d.current_speed;
            return; // 激活成功，撤退
        }
    }
}

void DanmakuPool::update(float dt) {
    for (auto& d : pool) {
        if (!d.active) continue;

        // 1. 指令流状态机解析
        if (d.trajectory_id < trajectories.size()) {
            const auto& traj = trajectories[d.trajectory_id];
            
            if (d.current_cmd_index < traj.commands.size()) {
                const auto& cmd = traj.commands[d.current_cmd_index];
                
                switch (cmd.type) {
                    case DanmakuCmdType::WAIT:
                        d.cmd_timer += dt;
                        if (d.cmd_timer >= cmd.param1) {
                            d.current_cmd_index++; // 时间到，执行下一条
                            d.cmd_timer = 0.0f;
                        }
                        break;
                    case DanmakuCmdType::SET_SPEED:
                        d.current_speed = cmd.param1;
                        d.current_cmd_index++;
                        break;
                    case DanmakuCmdType::TURN_ANGLE:
                        d.current_angle += cmd.param1 * (3.14159f / 180.0f); // 角度转弧度
                        d.current_cmd_index++;
                        break;
                    case DanmakuCmdType::ACCELERATE:
                        d.current_speed += cmd.param1 * dt;
                        d.cmd_timer += dt;
                        if (d.cmd_timer >= cmd.param2) { // param2 设为加速持续时间
                            d.current_cmd_index++;
                            d.cmd_timer = 0.0f;
                        }
                        break;
                    case DanmakuCmdType::SMOOTH_TURN:

                        // param1 = 需要转动的总角度 (例如 90 度，记得转成弧度)
                        // param2 = 转向耗费的总时间 (例如 1.0 秒)
                        
                        // 计算当前的角速度 (单位：弧度/秒)
                        float angular_speed = (cmd.param1 * (3.14159265f / 180.0f)) / cmd.param2;
                        
                        // 推进角度
                        d.current_angle += angular_speed * dt;
                        
                        // 累加时间
                        d.cmd_timer += dt;
                        
                        // 检查这个动作是否做完了
                        if (d.cmd_timer >= cmd.param2) {
                            // 动作完成！进入下一条指令
                            d.current_cmd_index++;
                            d.cmd_timer = 0.0f;
                            
                            // 【防微小误差战术】浮点数相加必有误差，为了防止最后偏了几度，
                            // 我们最好在这里直接把角度 snap (咬合) 到极其精确的目标值。
                            // （不过对于简单的弹幕游戏，不写这段 snap 也没事，直接进入下一条即可）
                        }
                        break;

                }
            }
        }

        // 2. 物理运动推演
        d.vel.x = std::cos(d.current_angle) * d.current_speed;
        d.vel.y = std::sin(d.current_angle) * d.current_speed;
        d.pos += d.vel * dt;

        // 3. 屏幕边界剔除 (根据你设置的 800x600，稍微留点缓冲区)
        if (d.pos.x < -100 || d.pos.x > 900 || d.pos.y < -100 || d.pos.y > 700) {
            d.active = false;
        }
    }
}

void DanmakuPool::render(SDLRender& renderer) {
    auto res_mgr = ResourcesManager::getInstance();

    for (const auto& d : pool) {
        if (!d.active) continue;

        const auto& prefab = prefabs[d.prefab_id];
        Texture* tex = res_mgr->get_danmaku_texture(prefab.texture_id);

        // 如果需要子弹朝向运动方向，可以直接传入 d.current_angle (转换成角度)
        double render_angle = d.current_angle * (180.0 / 3.1415926535);

        renderer.draw_texture(
            tex, 
            d.pos, 
            prefab.base_scale, 
            prefab.base_size, 
            render_angle, 
            255 // alpha 默认不透明，后续可加到 d 中
        );
    }
}

void DanmakuPool::check_player_hit(Player* player) {
    if (!player || !player->is_alive()) return;

    glm::vec2 p_pos = player->get_position();
    float p_radius = 2.0f; // 核心判定点

    for (auto& d : pool) {
        if (!d.active) continue;

        const auto& prefab = prefabs[d.prefab_id];
        
        if (prefab.shape_type == DanmakuShape::Circle) {
            float dx = d.pos.x - p_pos.x;
            float dy = d.pos.y - p_pos.y;
            float dist_sq = dx * dx + dy * dy;
            float hit_r = prefab.base_radius + p_radius;

            if (dist_sq <= hit_r * hit_r) {
                player->set_alive(false);
                d.active = false;
                std::cout << "[Core] Player hit by Danmaku!" << std::endl;
                // 后续可以触发事件系统
            }
        }
    }
}