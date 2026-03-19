#pragma once

#include"IWindows.h"
#include<SDL.h>
#include<iostream>


class SDLWindows
{
public:
    SDLWindows() = default;
    ~SDLWindows(){

        if(this->window != nullptr){
            SDL_DestroyWindow(this->window);
            this->window = nullptr;
        }
    };

    virtual bool init(int w,int h);

    SDL_Window* get_sdl_window();

private:
    SDL_Window* window;
};