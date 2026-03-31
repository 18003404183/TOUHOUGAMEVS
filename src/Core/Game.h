#pragma once
#include "IWindows.h"
#include "ResourcesManager.h"
#include "SceneManager.h"
#include "includes.h"
// #include"EasyxWindow.h"
// #include"EasyxRender.h"
#include "SDLRender.h"
#include "SDLWindow.h"
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

class Game {
public:
    Game();
    ~Game();

    bool init_game();
    void load_resources();
    void start();
    void end();

private:
    SDLRender renderer_;
    std::unique_ptr<SDLWindows> window_;
};