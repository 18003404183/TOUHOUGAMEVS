#pragma once

#include <functional>

class Clock {
public:
    // 构造：默认 1000 ms，非一次性，repeat=-1 表示无限重复，未激活
    Clock()
        : max_time(1000), one_shoot(false), repeat_time(-1), call_back(nullptr), current_time(0), is_available(1) {}
    ~Clock() = default;

    // 设置/获取最大时间（毫秒）
    void setMaxTime(int ms) { max_time = (ms > 0 ? ms : 0); }
    int getMaxTime() const { return max_time; }

    // 是否一次触发
    void setOneShoot(bool v) { one_shoot = v; }
    bool isOneShoot() const { return one_shoot; }

    // 重复次数（-1 表示无限重复）
    void setRepeatTime(int times) { repeat_time = times; }
    int getRepeatTime() const { return repeat_time; }

    // 设置回调
    void setCallback(std::function<void()> cb) { call_back = std::move(cb); }

    // 启动/暂停/恢复/重置
    void start() { current_time = 0; is_available = 1; }
    void stop() { is_available = 0; }
    void pause() { is_available = 0; }
    void resume() { is_available = 1; }
    void reset() { current_time = 0; }

    // 查询
    bool available() const { return is_available != 0; }
    int currentTime() const { return current_time; }
    int remainingTime() const { int rem = max_time - current_time; return (rem > 0 ? rem : 0); }

    // 减少时间（等同于 advance）
    void reduce(int ms) { update(ms); }

    // 更新闹钟（传入增量毫秒）
    void update(int delta_ms) {
        if (!available() || max_time <= 0 || delta_ms <= 0) return;
        current_time += delta_ms;

        // 处理可能跨越多个周期的情况
        while (current_time >= max_time && available()) {
            if (call_back) call_back();

            if (one_shoot) {
                is_available = 0;
                break;
            }

            if (repeat_time > 0) {
                --repeat_time;
                if (repeat_time == 0) { is_available = 0; break; }
            }

            current_time -= max_time;
        }
    }

private:
    // 最大时间（毫秒）
    int max_time;
    // 是否一次性触发
    bool one_shoot;
    // 重复次数（-1 表示无限）
    int repeat_time;
    // 回调
    std::function<void()> call_back;
    // 当前累积时间（毫秒）
    int current_time;
    // 激活标志（0/非0）
    int is_available;
};