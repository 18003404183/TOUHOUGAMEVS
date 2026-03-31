#pragma once

#include"Player.h"
#include"SceneType.h"
#include"IScene.h"
#include"MainScene.h"
#include"EndScene.h"
#include"SDLRender.h"

class SceneManager
{
public:
    static SceneManager* get_instance();
    void load_scene(SceneType scene_type);
    void unload_scene();
    void switch_scene(SceneType scene_type);
    void on_update(float delta_time);
    void on_input();
    void on_render(SDLRender& renderer);
    bool current_scene_null();
    SceneManager(SceneManager& scene_manager) = delete;
    SceneManager& operator=(SceneManager& scene_manager) = delete;

private:
    SceneManager() = default;
    ~SceneManager() = default;
    IScene* current_scene_;

    std::unique_ptr<MainScene> main_scene_ = std::make_unique<MainScene>();
    std::unique_ptr<EndScene> end_scene_ = std::make_unique<EndScene>();

    static SceneManager* scene_manager_;
};