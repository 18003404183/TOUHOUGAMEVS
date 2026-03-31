#include "SceneManager.h"

SceneManager* SceneManager::scene_manager_ = nullptr;

SceneManager* SceneManager::get_instance() {
	if (scene_manager_ == nullptr) {
		scene_manager_ = new SceneManager();
	}
	return scene_manager_;
}

void SceneManager::load_scene(SceneType scene_type)
{
	if (scene_type == SceneType::MainMenu)
	{
		this->current_scene_ = this->main_scene_.get();
		current_scene_->on_enter();
	}
}

void SceneManager::unload_scene()
{
	current_scene_->on_exit();
	this->current_scene_ = nullptr;
}

void SceneManager::switch_scene(SceneType scene_type)
{
	current_scene_->on_exit();
	if (scene_type == SceneType::MainMenu) {
		this->current_scene_ = this->main_scene_.get();
	}
	if (scene_type == SceneType::EndMenu) {
		this->current_scene_ = this->end_scene_.get();
	}
	this->current_scene_->on_enter();
}

void SceneManager::on_update(float delta_time)
{
	this->current_scene_->on_update(delta_time);

}

void SceneManager::on_input() {
	this->current_scene_->on_input();
}

void SceneManager::on_render(SDLRender& renderer) {
	this->current_scene_->on_render(renderer);
}

bool SceneManager::current_scene_null(){
	return !current_scene_;
}