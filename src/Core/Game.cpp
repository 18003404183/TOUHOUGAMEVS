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
	//EasyxWindow easyx_window;
	//easyx_window.init();
	SDLWindows* windows = new SDLWindows;
	windows->init(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
	
	this->renderer = new SDLRender();
	renderer->init(windows->get_sdl_window());
	
	ResourcesManager::getInstance()->set_renderer(this->renderer);
	SceneManager::getInstance()->loadScene(SceneType::MainMenu);


	return true;
}

void Game::load_resources()
{
	std::cout << "Resources loaded(Game)" << std::endl;
	ResourcesManager::getInstance()->loadSceneTotal("1","json");
	ResourcesManager::getInstance()->loadScene(SceneType::MainMenu);
}

void Game::start()
{
	std::cout << "Game started" << std::endl;
	SDL_Event event;
	bool is_running = true;
	SceneManager* scene_manager = SceneManager::getInstance();
	Uint32 frameStart;
    int frameTime;
	while (is_running)
	{
		//std::cout << "Game_running" << std::endl;
		frameStart = SDL_GetTicks();
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				is_running = false;
			}
			if(!scene_manager->current_scene_null()){
				//把事件传输给场景处理
				scene_manager->on_input();
			}
		}		
		scene_manager->on_update(1000 / GAME_TPS);
		this->renderer->clear();
		scene_manager->on_render(renderer);
		this->renderer->present();

		frameTime = SDL_GetTicks() - frameStart;
		if (1000/GAME_TPS > frameTime) {
            SDL_Delay(1000/GAME_TPS - frameTime);
        }
	}

}

void Game::end(){
	delete this->renderer;
	return;

}