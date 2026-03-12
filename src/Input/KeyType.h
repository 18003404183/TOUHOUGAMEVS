#pragma once
#include "includes.h"
#include <SDL.h> // 必须引入 SDL

enum class KeyType
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    Z,
    X,
    C,
    SHIFT,
    CTRL,
    ENTER,
    _count
};

// 战术核心：直接映射到 SDL 的底层键盘扫描码
const SDL_Scancode Keys[] = { 
    SDL_SCANCODE_UP, 
    SDL_SCANCODE_DOWN, 
    SDL_SCANCODE_LEFT, 
    SDL_SCANCODE_RIGHT, 
    SDL_SCANCODE_Z, 
    SDL_SCANCODE_X, 
    SDL_SCANCODE_C, 
    SDL_SCANCODE_LSHIFT, 
    SDL_SCANCODE_LCTRL, 
    SDL_SCANCODE_RETURN 
};