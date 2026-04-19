// 发射器负责按照一定的时间空间顺序生成弹幕 作为一个弹幕生成接口被外部调用(当发射器只发射一次时 也就是最常见的射出一次子弹 时间顺序退化为立刻 在时效到达上线后 发射器设为完成任务)
// 发射器有两种确定自身坐标的方式 世界的绝对位置 和 绑定的父节点的位置
// 发射器目前只考虑一种生成弹幕坐标的方式 即从自己的位置+偏移量 开始生成弹幕
// 发射器中含有一个clock 来保证弹幕按照时间次序发射 并且在完成使命后将自己状态设为finish 一般不存在一直生成弹幕的情况 但是仍然提供不断生成弹幕的接口 并且提供外部将自己设为finish的接口 以更灵活的控制弹幕的发射
// 发射器有三种确定发射方向的方式 一种是写死(初始角度从零开始按照需要的方式递增) 一种是将初始角度设为指向指定实体位置 然后按照方式递增 一种是始终指向指定目标实体/地址

// 确定发射器自身坐标 确定发射点偏移 就能确定发射弹幕的位置 确定发射的位置和方向 就可以生成所有需要的弹幕发射方式
// 与弹幕轨迹指令系统配合 应该能够解决大多数情况

#pragma once
#include "Danmaku.h"
#include "clock.h"
#include <glm.hpp>
#include <cmath>
#include <functional>

// 发射器的坐标空间
enum class EmitterSpace {
    Relative, // 相对坐标（绑定父节点）
    Absolute  // 绝对坐标（世界坐标）
};

// 瞄准模式
enum class AimMode {
    Fixed,         // 写死：初始角度固定，每次累加 angle_step
    TargetInitial, // 初始索敌：生成第一颗子弹时瞄准目标，后续在这个基础角度上累加 angle_step
    TargetAlways   // 持续索敌：每一颗子弹生成时，都重新计算指向目标的角度，并叠加 angle_step
};

class Emitter {
private:
    // --- 空间坐标系统 ---
    EmitterSpace space;
    const glm::vec2* parent_pos; // 绑定的父节点坐标 (如果不绑定则为 nullptr)
    glm::vec2 offset;            // 偏移量

    // --- 瞄准系统 ---
    AimMode aim_mode;
    const glm::vec2* target_pos; // 目标实体的坐标 (用于自机狙，不索敌则为 nullptr)
    float locked_initial_angle;  // 用于 TargetInitial 模式锁定初始角度

    // --- 弹幕属性 ---
    uint16_t prefab_id;
    float speed;
    float base_angle;            // 基础角度 (或瞄准偏移角)
    float angle_step;            // 每次发射偏转角度 (做螺旋、散弹用)

    // --- 时间与状态系统 ---
    Clock clock;
    int total_count;             // 总发射次数 (-1 为无限)
    int spawned_count;           // 已发射次数
    bool is_finished_flag;
    bool is_first_update;        // 标记是否是第一帧，用于实现“立刻发射第一颗”

    // 临时挂载的池指针，供 Clock 回调函数使用
    DanmakuPool* current_pool = nullptr;

    // Clock 触发时的核心回调逻辑
    void fire_callback() {
        if (!current_pool || is_finished_flag) return;
        glm::vec2 spawn_pos = get_real_position();
        float final_angle = base_angle;

        // 核心：根据不同模式计算最终发射角度
        switch (aim_mode) {
            case AimMode::Fixed:
                final_angle = base_angle + (spawned_count * angle_step);
                break;

            case AimMode::TargetInitial:
                // 第一次发射时，锁定指向目标的角度
                if (spawned_count == 0 && target_pos != nullptr) {
                    locked_initial_angle = std::atan2(target_pos->y - spawn_pos.y, target_pos->x - spawn_pos.x);
                }
                // 后续都在锁定角度的基础上累加
                final_angle = locked_initial_angle + base_angle + (spawned_count * angle_step);
                break;

            case AimMode::TargetAlways:
                // 每次发射都重新计算目标角度
                if (target_pos != nullptr) {
                    float current_aim = std::atan2(target_pos->y - spawn_pos.y, target_pos->x - spawn_pos.x);
                    final_angle = current_aim + base_angle + (spawned_count * angle_step);
                } else {
                    // 如果目标丢了，退化为 Fixed 模式防崩溃
                    final_angle = base_angle + (spawned_count * angle_step); 
                }
                break;
        }

        // 发射！
        current_pool->spawn(prefab_id, spawn_pos, speed, final_angle);
        spawned_count++;

        // 安全检查：如果达到总数，强行结束（虽然 Clock 也会停）
        if (total_count > 0 && spawned_count >= total_count) {
            finish();
        }
    }

public:
    // 全能构造函数
    Emitter(EmitterSpace space, const glm::vec2* parent, glm::vec2 offset,
            AimMode aim_mode, const glm::vec2* target,
            uint16_t prefab, float speed, float base_angle, float angle_step,
            float interval, int count)
        : space(space), parent_pos(parent), offset(offset),
          aim_mode(aim_mode), target_pos(target), locked_initial_angle(0.0f),
          prefab_id(prefab), speed(speed), base_angle(base_angle), angle_step(angle_step),
          total_count(count), spawned_count(0), is_finished_flag(false), is_first_update(true) 
    {
        // 1. 设置 Clock 时间间隔
        // 为了防止传入 0 导致 Clock 直接 return，如果需要顺发多颗，给一个极小的时间
        clock.set_max_time(interval <= 0.0f ? 0.0001f : interval); 
        
        // 2. 设置 Clock 重复次数
        if (count == 1) {
            clock.set_one_shoot(true);
        } else if (count > 1) {
            clock.set_one_shoot(false);
            clock.set_repeat_time(count - 1); // 发 5 颗，等于触发 1 次 + 重复 4 次
        } else {
            clock.set_one_shoot(false);
            clock.set_repeat_time(-1); // 无限重复
        }

        // 3. 绑定回调函数
        clock.set_callback([this]() { this->fire_callback(); });
        
        clock.start();
    }

    // 获取真实坐标
    glm::vec2 get_real_position() const {
        if (space == EmitterSpace::Relative && parent_pos != nullptr) {
            return *parent_pos + offset;
        }
        return offset;
    }

    // 外部主动销毁/停止接口
    void finish() {
        is_finished_flag = true;
        clock.stop();
    }

    bool is_finished() const {
        return is_finished_flag || !clock.available();
    }

    // 更新引擎
    void update(float dt, DanmakuPool* pool) {
        if (is_finished()) return;

        // 挂载资源池供 callback 内部使用
        this->current_pool = pool;
        // 这里在回调的时候发生了问题 回调函数绑定了那个构造时(在mainscene)构建的零时对象 导致现在出现了空指针(vector默认是拷贝赋值)

        // 补丁：让第一颗子弹在第一帧瞬间生成，而不是傻等一个 interval
        if (is_first_update) {
            is_first_update = false;
            // 强推时间轴，立刻触发一次回调！
            clock.update(clock.get_max_time()); 
        }

        // 正常更新时钟
        clock.update(dt);

        // 卸载资源池，保证内存安全
        this->current_pool = nullptr;

        // 同步状态
        if (!clock.available()) {
            is_finished_flag = true;
        }
    }

    // 动态控制发射器的开关 (用于绑定玩家按键)
    void set_active(bool active) {
        if (active) {
            // 如果要求激活，且当前闹钟处于非激活状态，则恢复
            if (!clock.available()) clock.resume();
        } else {
            // 如果要求停止，且当前闹钟正在运行，则暂停
            if (clock.available()) clock.pause();
        }
    }
};