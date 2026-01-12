#pragma once

#include <functional>

class Clock {
public:
    // 构造：默认 1 s，非一次性，repeat=-1 表示无限重复，未激活
    Clock()
        : max_time(1), one_shoot(false), repeat_time(-1), call_back(nullptr), current_time(0), is_available_(1),is_triggrted_(false) {}
    ~Clock() = default;

    // 设置/获取最大时间（秒）
    void set_max_time(float s) { max_time = (s > 0 ? s : 0); }
    float get_max_time() const { return max_time; }

    // 是否一次触发
    void set_one_shoot(bool v) { one_shoot = v; }
    bool is_one_shoot() const { return one_shoot; }

    // 重复次数（-1 表示无限重复）
    void set_repeat_time(int times) { repeat_time = times; }
    int get_repeat_time() const { return repeat_time; }

    // 设置回调
    void set_callback(std::function<void()> cb) { call_back = std::move(cb); }

    // 启动/暂停/恢复/重置
    void start() { current_time = 0; is_available_ = 1; }
    void stop() { is_available_ = 0; }
    void pause() { is_available_ = 0; }
    void resume() { is_available_ = 1; }
    void reset() { current_time = 0; }

    // 查询
    bool available() const { return is_available_ != 0; }
    float get_current_ime() const { return current_time; }
    float get_remaining_time() const { float rem = max_time - current_time; return (rem > 0 ? rem : 0); }

    bool is_triggrted(){

        return this->is_triggrted_;
    }

    // 减少时间（等同于 advance）
    void reduce(float s) { update(s); }

    // 更新闹钟（传入增量毫秒）
    void update(float delta_ms) {
        if (!available() || max_time <= 0 || delta_ms <= 0) return;
        current_time += delta_ms;

        // 处理可能跨越多个周期的情况
        while (current_time >= max_time && available()) {
            this->is_triggrted_ = true;
            if (call_back){
                call_back();
                //std::cout<<"回调函数调用11111"<<std::endl;
            }

            if (one_shoot) {
                is_available_ = 0;
                is_triggrted_ = false;
                break;
            }

            if (repeat_time > 0) {
                --repeat_time;
                if (repeat_time == 0) { is_available_ = 0; break; }
            }

            current_time -= max_time;
            this->is_triggrted_ = false;
        }

    }

private:
    // 最大时间（秒）
    float max_time;
    // 是否一次性触发
    bool one_shoot;
    // 重复次数（-1 表示无限）
    int repeat_time;
    // 回调
    std::function<void()> call_back;
    // 当前累积时间（毫秒）
    float current_time;
    // 激活标志（0/非0）
    int is_available_;

    //当前闹钟是否可触发 用来方便轮询
    bool is_triggrted_;
};