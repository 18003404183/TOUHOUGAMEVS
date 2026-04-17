#pragma once

#include<sol/sol.hpp>
#include<string>

class BossScript{
private:
    sol::state lua;
    sol::thread lua_thread;
    sol::coroutine boss_co;
    bool is_running = false;

public:
    void init(){

        lua.open_libraries(sol::lib::base,sol::lib::math,sol::lib::coroutine);

        lua.set_function("print_debug",[](const std::string& msg){
            std::cout << "[Lua_Boss]" << msg << std::endl;
        });

        try
        {
            lua.script_file("resources/scripts/boss_test.lua");
        }
        catch(const sol::error& e)
        { 
            std::cerr << "脚本加载失败因为" << e.what() << '\n';
        }

        lua_thread = sol::thread::create(lua.lua_state());
        sol::function logic_func = lua["boss_logic"];
        boss_co = sol::coroutine(lua_thread.state(),logic_func);
        if (!boss_co.valid()) {
            std::cout << "[C++ 诊断] 惨了，协程绑定失败！" << std::endl;
        } else {
            std::cout << "[C++ 诊断] 协程绑定成功！油门已备好！" << std::endl;
        }
        is_running = true;

    }

    void update(float delta_time){

        if(!is_running) return;

        if(boss_co.valid()){
            auto result = boss_co(delta_time);
            if(!result.valid()){
                sol::error err = result;
                std::cerr << "剧本运行时报错" << err.what() << std::endl;
                this->is_running = false;
            }
        }
        else{
            std::cout << "[Lua] Boss剧本执行完毕" << std::endl;
            is_running = false;
        }
    }


};