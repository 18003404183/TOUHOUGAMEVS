#include"WaveController.h"
#include<json.hpp>
#include"fstream"
#include<iostream>
#include"EntityConfig.h"

using json = nlohmann::json;

void WaveController::update(float delta_time){
    this->current_time_ += delta_time;
    if(current_time_ >= this->wave_infos_[this->current_wave].spawn_time){
        this->has_data_need_create = true;     
    }
}

bool WaveController::load_json(std::string path){

    this->wave_infos_.clear();
    this->current_wave = 0;

    std::ifstream file;
    file.open(path);

    if(!file.is_open()){
        std::cout << "波次信息文件加载失败\n";
        return false;
    }

    json data;
    try{
        file >> data;
    }
    catch(const json::parse_error& e){
        std::cout << "json解析错误 原因为" << e.what();
        return false;
    }

    if(data.contains("waves")){
       for(const auto& wave : data["waves"]){
            WaveInfo temp_info;
            temp_info.spawn_time = wave.value("spawn_time",0.0f);
            for(const auto& enemy : wave["enemies"]){
                EnemyCreateData temp_data;
                temp_data.name = enemy.value("type",""); 
                temp_data.pos.x = enemy.value("x",0.0f);
                temp_data.pos.y = enemy.value("y",0.0f);
                temp_info.spawn_datas.push_back(temp_data);
            }

            this->wave_infos_.push_back(temp_info);
       } 
    }
    return true;
}  

std::vector<EnemyCreateData> WaveController::get_create_data(){
    std::vector<EnemyCreateData> create_list;
    if(!has_data_need_create){
        return create_list;
    }

    while(this->wave_infos_[this->current_wave].spawn_time <= current_time_ && this->current_wave < this->wave_infos_.size()){
        for(auto create_data : wave_infos_[this->current_wave].spawn_datas){
            create_list.push_back(create_data);
        }
        this->current_wave ++;
    }

    this->has_data_need_create = false;

    return create_list;
}