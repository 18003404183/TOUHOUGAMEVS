#include "Game.h"
#include "Config.h"

Game::Game()
{
	std::cout << "Game created" << std::endl;
}

Game::~Game()
{
	std::cout << "Game destroyed" << std::endl;
}

bool Game::init_game()
{
	std::cout << "Game initialized" << std::endl;
	ResourcesManager::getInstance();
	SceneManager::getInstance()->loadScene(SceneType::MainMenu);
	//EasyxWindow easyx_window;
	//easyx_window.init();
	SDLWindows* windows = new SDLWindows;
	windows->init(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
	
	SDLRender* renderer = new SDLRender();
	renderer->init(windows->get_sdl_window());

	return true;
}

void Game::load_resources()
{
	std::cout << "Resources loaded(Game)" << std::endl;
	ResourcesManager::getInstance()->loadSceneTotal("1","json");
	ResourcesManager::getInstance()->loadScene("SceneType::MainMenu","json");
}

void Game::start()
{
	std::cout << "Game started" << std::endl;
	time_t t;

	while (true)
	{
		
		std::cout << "Game_running" << std::endl;
		SceneManager* scene_manager = SceneManager::getInstance();
		scene_manager->on_input();
		scene_manager->on_update(1000 / GAME_TPS);
		scene_manager->on_render();

		t = clock();
		while ((int)(clock() - t) < 1000 / GAME_TPS)
		{

		}
	}

}

void Game::end(){

	return;

}