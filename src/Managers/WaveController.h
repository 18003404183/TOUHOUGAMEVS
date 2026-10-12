#pragma once

#include<vector>
#include<string>
#include<glm.hpp>

struct EnemyCreateData{
    std::string name;
    glm::vec2 pos = {0,0};    
};

struct WaveInfo{
    float spawn_time = 0;
    std::vector<EnemyCreateData> spawn_datas;
};

class WaveController{
private:
    float current_time_ = 0;
    int current_wave = 0;
    //这里默认json里的波次按照出现顺序排列 即默认生成时间从小到大 如果json并非按照时间顺序编写会导致波次混乱
    std::vector<WaveInfo> wave_infos_;
    bool has_data_need_create = false;
public:
    void update(float delta_time); 
    bool load_json(std::string path);   
    std::vector<EnemyCreateData> get_create_data();
};