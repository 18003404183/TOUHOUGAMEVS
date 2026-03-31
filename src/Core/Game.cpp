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
    ResourcesManager::get_instance();
    
    window_ = std::make_unique<SDLWindows>();
    window_->init(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    
    renderer_ = SDLRender();
    renderer_.init(window_->get_sdl_window());
    
    ResourcesManager::get_instance()->set_renderer(&renderer_);
    load_resources();
    SceneManager::get_instance()->load_scene(SceneType::MainMenu);

    Context::instances()->init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    // 绑定 SDL 窗口和渲染器后端
    ImGui_ImplSDL2_InitForSDLRenderer(window_->get_sdl_window(), renderer_.get_renderer());
    ImGui_ImplSDLRenderer2_Init(renderer_.get_renderer());

    return true;
}

void Game::load_resources()
{
    std::cout << "Resources loaded(Game)" << std::endl;
    ResourcesManager::get_instance()->load_scene_total("1", "json");
}

void Game::start()
{
    std::cout << "Game started" << std::endl;
    SDL_Event event;
    bool is_running = true;
    SceneManager* scene_manager = SceneManager::get_instance();
    uint64_t frame_start = SDL_GetTicks64();
    uint64_t frame_end = SDL_GetTicks64();
    uint64_t frame_time;
    
    while (is_running)
    {
        float delta_time = frame_end - frame_start;
        frame_start = frame_end;
        
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
        }

        IInput::update();

        if (!scene_manager->current_scene_null())
        {
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
        ImGui::Text("Game Frame: %d", Context::instances()->game_frame);
        ImGui::End();
        
        scene_manager->on_update(1.0 / GAME_TPS);
        renderer_.clear();
        scene_manager->on_render(renderer_);

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_.get_renderer());

        renderer_.present();

        frame_time = SDL_GetTicks() - frame_start;
        if (1000 / GAME_TPS > frame_time)
        {
            SDL_Delay(1000 / GAME_TPS - frame_time);
        }
        frame_end = SDL_GetTicks64();
        int frame = Context::instances()->game_frame++;
        std::cout << "current_frame: " << frame << std::endl;
    }
}

void Game::end()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}