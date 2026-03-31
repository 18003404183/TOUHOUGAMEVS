#pragma once
#include "includes.h"
#include "Collider.h"

struct PlayerContext
{
    glm::vec2 position{0, 0};
    glm::vec2 velocity{0, 0};
    bool active = true;
    bool is_alive = true;
    int hp = 100;
};

class Context
{
public:
    static Context* instances()
    {
        if (!context_)
        {
            context_ = new Context();
        }
        return context_;
    }

    void init()
    {
        game_frame = 0;
    }

    void set_player_context(glm::vec2 position,
                            glm::vec2 velocity,
                            bool active,
                            bool is_alive,
                            int hp)
    {
        player_context_.position = position;
        player_context_.active = active;
        player_context_.hp = hp;
        player_context_.is_alive = is_alive;
        player_context_.velocity = velocity;
    }

    void set_player_context(PlayerContext pc)
    {
        player_context_ = pc;
    }

    PlayerContext get_player_context()
    {
        return player_context_;
    }

    Context(const Context& context) = delete;
    Context& operator=(const Context& b) = delete;

    int game_frame = 0;

private:
    PlayerContext player_context_;
    Context() = default;
    static Context* context_;
};