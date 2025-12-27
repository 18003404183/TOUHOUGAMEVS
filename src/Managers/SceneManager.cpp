#include "SceneManager.h"

SceneManager* SceneManager::scene_manager = nullptr;

SceneManager* SceneManager::getInstance() {
	if (scene_manager == nullptr) {
		scene_manager = new SceneManager();
	}
	return scene_manager;
}

void SceneManager::loadScene(SceneType scenetype)
{
	if (scenetype == SceneType::MainMenu)
	{
		this->current_scene = this->main_scene;
		current_scene->on_enter();
	}
}

void SceneManager::unloadScene()
{
	current_scene->on_exit();
	this->current_scene = nullptr;
}

void SceneManager::switchScene(SceneType scenetype)
{
	current_scene->on_exit();
	if (scenetype == SceneType::MainMenu) {
		this->current_scene = this->main_scene;
	}
	if (scenetype == SceneType::EndMenu) {
		this->current_scene = this->end_scene;
	}
	this->current_scene->on_enter();
}

void SceneManager::on_update(float deltatime)
{
	this->current_scene->on_update(deltatime);

}

void SceneManager::on_input() {
	this->current_scene->on_input();
}

void SceneManager::on_render(SDLRender* renderer) {
	this->current_scene->on_render(renderer);
}

bool SceneManager::current_scene_null(){
	return !current_scene;
}