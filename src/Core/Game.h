#pragma once
#include"includes.h"
#include"ResourcesManager.h"
#include"SceneManager.h"
#include"IWindows.h"
#include"EasyxWindow.h"
#include"EasyxRender.h"
#include"SDLRender.h"
#include"SDLWindow.h"
#include<SDL.h>


class Game
{
public:
	Game();
	~Game();
	
	bool init_game();
	void load_resources();
	void start();
	void end();

private:

	SDLRender renderer;
};

