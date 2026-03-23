#pragma once

#include"includes.h"
#include"SDL_render.h"
#include"ResourcesManager.h"

// 目前的思路是将弹幕作为一个单独的系统 采用纯粹的数据形式来存储弹幕 弹幕的渲染和更新交由弹幕管理系统完成
// 弹幕的渲染: 1.在resourcemanager单独开一条弹幕贴图的vector 不使用unorder_map 因为每次循环都要查询 需要考虑开销
// 2. 弹幕管理系统单独负责弹幕的渲染 参数接收一个渲染器再接收弹幕texture的id用于渲染 当然也可以重载更多的参数 能够自定义贴图大小 旋转等
// 对于数据的存储 采用享元 DanmakuData负责储存一个弹幕独有的信息(这个弹幕的类型 位置 速度 旋转角度 执行指令的时间 目前在执行第几个指令 是否存活等)
// DanmakuPrefab 用于存放一类弹幕公用的数据 比如渲染方面的 使用哪个贴图 渲染时如何渲染(初始旋转角度 缩放比例等) 更新方面的 比如执行哪个指令集
// 3 弹幕管理系统的更新 系统里写一个状态机 做一个指令结构体 储存指令类型 参数(比如相关指令执行的数量 执行持续多长时间) 然后将这些指令存入一个指令集 
// 每个弹幕通过读取指令集来形成不同的轨迹 每类弹幕共享一个指令集 弹幕自身存储自己对指令集的执行情况(执行了多长时间 执行到第几个)
// 弹幕管理器最终会通过指令的种类 各种参数等 来分辨该如何执行指令
// 4 碰撞的逻辑: 为类型单独写碰撞 比如一个函数接收实体指针(或存储实体指针的vector) 然后检测  
// 为了方便区分 可能会在场景里多维护一份enemy的队列 如果可能存在多玩家了话 可能需要多维护一份player队列
// 5 弹幕管理系统总结
// 弹幕管理系统维护几个vector 分别存储danmakudata danmakuprefab trajectories
// 弹幕管理系统提供接口创建以上三种类型 对于弹幕的处理依赖于内部对象池 外部只需要输入构建弹幕需要的参数便可
// 


// 碰撞体类型
enum class DanmakuShape : uint8_t{
    Circle,
    Rect,
    Laser
};

// 指令类型
enum class DanmakuCmdType {
    WAIT,
    SET_SPEED,
    TURN_ANGLE,
    ACCELERATE,
    SMOOTH_TURN
};

struct DanmakuPrefab{
    uint16_t texture_id;//存在资源管理器中的弹幕贴图队列中
    glm::vec2 base_size;     // 贴图原始大小
    glm::vec2 base_scale;    // 默认缩放
    DanmakuShape shape_type; // 碰撞形状
    float base_radius;       // 基础碰撞判定半径
    uint16_t default_traj_id;// 默认绑定的轨迹模板 ID
};

struct DanmakuData{
    glm::vec2 pos;
    glm::vec2 vel;
    float current_speed;
    float current_angle;

    uint16_t prefab_id;//预制菜id
    uint16_t trajectory_id;//轨迹id

    uint8_t current_cmd_index;
    float cmd_timer;

    bool active;

};

// 指令
struct DanmakuCommand
{
    DanmakuCmdType type;
    float param1; //参数1
    float param2; // 参数2
};

//指令队列 / 轨迹模板
struct TrajectoryTemplate
{
    std::vector<DanmakuCommand> commands;
};

class DanmakuPool{
public:
    DanmakuPool() = delete;
    DanmakuPool(size_t pool_size);
    ~DanmakuPool() = default;

    
    // 对象池不可拷贝 
    DanmakuPool(const DanmakuPool& other) = delete;
    DanmakuPool operator=(const DanmakuPool& other) = delete;

    //用于注册
    static uint16_t register_prefab(const DanmakuPrefab& prefab);
    static uint16_t register_trajectory(const TrajectoryTemplate& traj);

    // 用于生成子弹
    void spawn(uint16_t prefab_id, glm::vec2 start_pos, float initial_speed, float initial_angle);

    //各种执行阶段
    void update(float delta);
    void render(SDLRender& renderer);

    void clear_pool();

    std::vector<DanmakuData>& get_raw_pool();
    static DanmakuPrefab get_prefab(int id);

    void destroy_bullet(DanmakuData& danmaku);
    int count;

private:
    std::vector<DanmakuData> pool;
    static std::vector<DanmakuPrefab> prefabs;
    static std::vector<TrajectoryTemplate> trajectories;


};