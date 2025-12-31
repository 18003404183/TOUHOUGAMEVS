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
	static SceneManager* getInstance();
	void loadScene(SceneType scenetype);
	void unloadScene();
	void switchScene(SceneType scenetype);
	void on_update(float deltatime);
	void on_input();
	void on_render(SDLRender& renderer);
	bool current_scene_null();
	SceneManager(SceneManager& scene_manager) = delete;
	SceneManager& operator=(SceneManager& scene_manager) = delete;

private:

	SceneManager() =  default;
	~SceneManager() = default;
	IScene* current_scene;

	MainScene* main_scene = new MainScene();
	EndScene* end_scene = new EndScene();

	static SceneManager* scene_manager;

};

