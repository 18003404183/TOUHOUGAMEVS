#pragma once

#include "IWindows.h"
#include <SDL.h>
#include <iostream>

class SDLWindows
{
public:
    SDLWindows() = default;
    ~SDLWindows()
    {
        if (window_ != nullptr)
        {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
    }

    virtual bool init(int width, int height);

    SDL_Window* get_sdl_window();

private:
    SDL_Window* window_ = nullptr;
};