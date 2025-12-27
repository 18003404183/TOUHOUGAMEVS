#pragma once

#include"IWindows.h"
#include<SDL.h>
#include<iostream>


class SDLWindows
{
public:
    SDLWindows() = default;
    ~SDLWindows() = default;

    virtual bool init(int w,int h);

    SDL_Window* get_sdl_window();

private:
    SDL_Window* window;
};