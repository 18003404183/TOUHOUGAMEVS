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
	
	this->renderer=SDLRender();
	renderer.init(windows->get_sdl_window());
	
	ResourcesManager::getInstance()->set_renderer(&renderer);
	this->load_resources();
	SceneManager::getInstance()->loadScene(SceneType::MainMenu);

	Context::Instances()->init();


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     
    ImGui::StyleColorsDark();                             

    // 绑定 SDL 窗口和渲染器后端
    ImGui_ImplSDL2_InitForSDLRenderer(windows->get_sdl_window(), renderer.get_renderer());
    ImGui_ImplSDLRenderer2_Init(renderer.get_renderer());

	return true;
}

void Game::load_resources()
{
	std::cout << "Resources loaded(Game)" << std::endl;
	ResourcesManager::getInstance()->loadSceneTotal("1","json");
	//ResourcesManager::getInstance()->loadScene(SceneType::MainMenu);
}

void Game::start()
{
	std::cout << "Game started" << std::endl;
	SDL_Event event;
	bool is_running = true;
	SceneManager* scene_manager = SceneManager::getInstance();
	Uint64 frameStart = SDL_GetTicks64();
	Uint64 frameEnd = SDL_GetTicks64();
    Uint64 frameTime;
	while (is_running)
	{
		//std::cout << "Game_running" << std::endl;
	
		
		float dt = frameEnd - frameStart;
		frameStart = frameEnd;
		
		while(SDL_PollEvent(&event)){
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT){
				is_running = false;
			}

		}

		IInput::update();

		if(!scene_manager->current_scene_null()){
			//把事件传输给场景处理
			scene_manager->on_input();
		}
		// 开启 ImGui 新的一帧
		ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 绘制性能仪表盘 (Profiler)
        ImGui::Begin("Engine Profiler");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("Game Frame: %d", Context::Instances()->game_frame);
        ImGui::End();
		
		scene_manager->on_update(1.0/GAME_TPS);
		this->renderer.clear();
		scene_manager->on_render(renderer);

		ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get_renderer());

		this->renderer.present();

		frameTime = SDL_GetTicks() - frameStart;
		if (1000/GAME_TPS > frameTime) {
            SDL_Delay(1000/GAME_TPS - frameTime);
        }
		frameEnd = SDL_GetTicks64();
		int frame = Context::Instances()->game_frame++;
		std::cout<<"current_frame"<<frame<<std::endl;
	}

}

void Game::end(){
	//delete this->renderer;
	ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
	return;

}