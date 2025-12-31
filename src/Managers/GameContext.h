#pragma once
#include"includes.h"
#include"Image.h"

//如果一个字段是全局的 那么字段包含的所有数据也应该是全局的

struct PlayerContext
{
    glm::vec2 position{0,0};
	glm::vec2 velocity{0,0};
	bool active = true;
    bool is_alive;
	int hp;
	Image player_image;
};

class Context{
public:
    static Context* Instances(){
        if(!context){
            context = new Context();
        }
        return context;
    }

    void set_player_context(glm::vec2 position,
	glm::vec2 velocity,
	bool active,
    bool is_alive,
	int hp,
	Image player_image){
        this->pc.position = position;
        this->pc.active = active;
        this->pc.hp = hp;
        this->pc.is_alive = is_alive;
        this->pc.velocity = velocity;
        this->pc.player_image = player_image;
    }

    void set_player_context(PlayerContext pc){
        this->pc = pc;
    }

    PlayerContext get_player_context(){
        return pc;
    }

    Context(const Context& context) = delete;
    Context& operator=(const Context& b) = delete;

private:
    PlayerContext pc;
    Context() = default;
    static Context* context; 
};



